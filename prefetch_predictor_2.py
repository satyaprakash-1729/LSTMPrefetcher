import numpy as np
import keras
import json
from keras_self_attention import SeqSelfAttention
import os
import sys
from keras.utils import to_categorical
from keras import backend as K


os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"


def initialize_idx_to_delta(ROOT_DIR="./"):
  idx_to_delta = None
  ROOT_DIR="./"
  with open(ROOT_DIR + 'idx_delta_map.json') as json_file:
    idx_to_delta = json.load(json_file)
  return idx_to_delta

def initialize_delta_to_idx(ROOT_DIR="./"):
  delta_to_idx = None
  ROOT_DIR="./"
  with open(ROOT_DIR + 'delta_idx_map.json') as json_file:
    delta_to_idx = json.load(json_file)
  return delta_to_idx

def initialize_model(ROOT_DIR="./"):
  ROOT_DIR="./"
  f = open(ROOT_DIR + "model_for_cpp_gcc_self_attn.json")
  # K.clear_session()
  model = keras.models.model_from_json(f.read(), custom_objects={'SeqSelfAttention': SeqSelfAttention})
  f.close()
  model.load_weights(ROOT_DIR + "model_for_cpp_weights_gcc_self_attn.h5")
  model.compile(optimizer=keras.optimizers.Adam(lr=0.001), loss='categorical_crossentropy', metrics=['categorical_accuracy'])
  return model

def get_input_data(lastNData, N):
  def get_cluster_id(center1, center2, center3, point):
    arr = np.array([abs(center1-point), abs(center2-point), abs(center3-point)])
    return np.argmin(arr)

  if len(lastNData) < 2*N+6:
    return [], []

  lastNPCs = lastNData[:N]
  lastNAddrs = lastNData[N:2*N]
  topk = lastNData[2*N]
  idx_to_delta = lastNData[2*N+1]
  delta_to_idx = lastNData[2*N+2]
  model = lastNData[2*N+3]
  LOG2_PAGE_SIZE = lastNData[2*N+4]
  is_warmup = lastNData[2*N+5]

  ip1 = 4195632.0
  ip2 = 8208480.0

  addr11 = 96935917361984.0
  addr12 = 195743444536640.0

  addr21 = 5610901135424.0
  addr22 = 80904477544064.0

  addr31 = 222418542097984.0
  addr32 = 279264050879424.0

  center1 = 1.61729982e+14
  center2 = 3.12041036e+13
  center3 = 2.53453964e+14

  if is_warmup == 1:
    y = lastNAddrs[-1]-lastNAddrs[-2]

    ips = np.array(lastNPCs)[:-1]
    ips = [(ip-ip1)/(ip2-ip1) for ip in ips]

    addrs = np.array(lastNAddrs)[:-1]
    cluster_ids = [get_cluster_id(center1, center2, center3, addr) for addr in addrs]

    addrs[cluster_ids==0] = (addrs[cluster_ids==0] - addr11)/(addr12-addr11)
    addrs[cluster_ids==1] = (addrs[cluster_ids==1] - addr21)/(addr22-addr21)
    addrs[cluster_ids==2] = (addrs[cluster_ids==2] - addr31)/(addr32-addr31)

    X = np.array([list(xx) for xx in zip(ips, cluster_ids, addrs)])
    X = X.reshape(1, N-1, 3)
    return X, y, model, idx_to_delta, delta_to_idx, LOG2_PAGE_SIZE, topk
  else:
    ips = np.array(lastNPCs)[1:]
    ips = [(ip-ip1)/(ip2-ip1) for ip in ips]

    addrs = np.array(lastNAddrs)[1:]
    cluster_ids = [get_cluster_id(center1, center2, center3, addr) for addr in addrs]

    addrs[cluster_ids==0] = (addrs[cluster_ids==0] - addr11)/(addr12-addr11)
    addrs[cluster_ids==1] = (addrs[cluster_ids==1] - addr21)/(addr22-addr21)
    addrs[cluster_ids==2] = (addrs[cluster_ids==2] - addr31)/(addr32-addr31)

    X = np.array([list(xx) for xx in zip(ips, cluster_ids, addrs)])
    X = X.reshape(1, N-1, 3)
    return X, None, model, idx_to_delta, delta_to_idx, LOG2_PAGE_SIZE, topk

def prefetch_predict(lastNData, ROOT_DIR="./"):
  ROOT_DIR="./"
  N = 11
  is_warmup = lastNData[-1]
  if is_warmup == 0:
    X, _, model, idx_to_delta, _, LOG2_PAGE_SIZE, topk = get_input_data(lastNData, N)  
    y_pred = model.predict(X)

    idxs = y_pred.argsort()[0]
    topkidxs = []
    lastaddr = int(lastNData[2*N-1])
    for i in range(len(idxs)):
      if len(topkidxs) == 2*topk:
        break
      delta1 = int(idx_to_delta[str(idxs[-i-1])])
      if ((lastaddr + delta1) >> LOG2_PAGE_SIZE) == (lastaddr >> LOG2_PAGE_SIZE):
        topkidxs += [delta1, y_pred[0][idxs[-i-1]]]

    return topkidxs
  else:
    X, y, model, _, delta_to_idx, _, _ = get_input_data(lastNData, N)
    if str(y) not in delta_to_idx:
      return "Not Trained", y
    y = to_categorical(delta_to_idx[str(y)], num_classes=10).reshape(1, 10)
    model.fit(X, y, batch_size=1, epochs=1, shuffle=False, verbose=False)
    with open(ROOT_DIR + 'model_for_cpp_gcc_self_attn.json', 'w+') as fout:
        fout.write(model.to_json())
    model.save_weights(ROOT_DIR + 'model_for_cpp_weights_gcc_self_attn.h5', overwrite=True)
    return "Trained"
  
#idx_to_delta = initialize_idx_to_delta()
#delta_to_idx = initialize_delta_to_idx()
## delta_to_idx = {}
## for k, v in idx_to_delta.items():
##   delta_to_idx[v] = k
#
## with open('idx_delta_map.json', 'w') as f:
##   json.dump(delta_to_idx, f)
#model1 = initialize_model()
#
#args = []
#args += list(map(int, sys.argv[1].split(",")))
#args += list(map(int, sys.argv[2].split(",")))
#args += [int(sys.argv[3])]
#
#args += [idx_to_delta]
#args += [delta_to_idx]
#args += [model1]
#args += [int(sys.argv[4])]
#args += [int(sys.argv[5])]
#
#print(prefetch_predict(args))
#
