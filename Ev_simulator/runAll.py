import os
import itertools
import numpy as np

os.system('mkdir bin')
assert os.system('make') == 0
os.system('rm -rf results/*')


abc = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i'}
abc2 = {'e', 'f'}
confs2 = {
    '2a': {
        'mus': [1, 4],
        'gamma': 1.0/5,
        'alphas': [0.5, 0.5]
    },
    '2b': {
        'mus': [1, 4],
        'gamma': 1.0/5,
        'alphas': [0.75, 0.25]
    },
    '2c': {
        'mus': [1, 4],
        'gamma': 1.0/5,
        'alphas': [0.25, 0.75]
    },
    '2d': {
        'mus': [1, 4],
        'gamma': 1.0/10,
        'alphas': [0.5, 0.5]
    },
    '2e': {
        'mus': [1, 4],
        'gamma': 1.0/10,
        'alphas': [0.75, 0.25]
    },
    '2f': {
        'mus': [1, 4],
        'gamma': 1.0/10,
        'alphas': [0.25, 0.75]
    },
    '2g': {
        'mus': [1, 4],
        'gamma': 1.0/20,
        'alphas': [0.5, 0.5]
    },
    '2h': {
        'mus': [1, 4],
        'gamma': 1.0/20,
        'alphas': [0.75, 0.25]
    },
    '2i': {
        'mus': [1, 4],
        'gamma': 1.0/20,
        'alphas': [0.25, 0.75]
    }
}

confs4 = {
    '4a': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/5,
        'alphas': [0.25, 0.25, 0.25, 0.25]
    },
    '4b': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/5,
        'alphas': [0.4, 0.3, 0.2, 0.1]
    },
    '4c': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/5,
        'alphas': [0.1, 0.2, 0.3, 0.4]
    },
    '4d': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/10,
        'alphas': [0.25, 0.25, 0.25, 0.25]
    },
    '4e': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/10,
        'alphas': [0.4, 0.3, 0.2, 0.1]
    },
    '4f': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/10,
        'alphas': [0.1, 0.2, 0.3, 0.4]
    },
    '4g': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/20,
        'alphas': [0.25, 0.25, 0.25, 0.25]
    },
    '4h': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/20,
        'alphas': [0.4, 0.3, 0.2, 0.1]
    },
    '4i': {
        'mus': [1, 1.5, 3, 5],
        'gamma': 1.0/20,
        'alphas': [0.1, 0.2, 0.3, 0.4]
    }
}


os.system('mkdir results/probStates')
os.system('mkdir results/avgStats')
os.system('mkdir results/lossVsTime')
os.system('mkdir results/timeBoxplots')
os.system('mkdir results/lossBoxplots')
scenario = "BUFFER_ENGINE_4"

# states probabilities
resFolder = './results/probStates'
iters = 1
lambdas = [1, 1.5]  # np.arange(0.2, 2.2, 0.2)
npkts = 200000
buffers = [0, 8]  # np.arange(1, 65, 5)  #
confNames = ['4e', '4f']
muDists = ['EXP', 'CONST']

for confName, lamda,  buffer, muDist in itertools.product(confNames, lambdas, buffers, muDists):
    ft = '_{}.dat'.format(confName)
    conf = confs4[confName]
    fh = resFolder + '/{}_'.format(muDist)
    cmd = './bin/main {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} 0 1 0 0 0'.format(
        scenario, npkts, muDist, buffer, conf['alphas'][0], conf['alphas'][1], conf['alphas'][2], conf['alphas'][3],
        conf['mus'][0], conf['mus'][1], conf['mus'][2], conf['mus'][3], lamda, conf['gamma'], fh, ft, iters)
    print(cmd)
    os.system(cmd)

# total time & loss vs lambda
resFolder = './results/avgStats'
iters = 100
lambdas = np.arange(0.2, 2.2, 0.2)
npkts = 10000
buffers = [0, 4, 8, 16, 32]  # np.arange(1, 65, 5)  #
confNames = ['4e', '4f']
muDists = ['EXP', 'CONST']

for confName, lamda,  buffer, muDist in itertools.product(confNames, lambdas, buffers, muDists):
    ft = '_{}.dat'.format(confName)
    conf = confs4[confName]
    fh = resFolder + '/{}_'.format(muDist)
    cmd = './bin/main {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} 0 0 0 1 0'.format(
        scenario, npkts, muDist, buffer, conf['alphas'][0], conf['alphas'][1], conf['alphas'][2], conf['alphas'][3],
        conf['mus'][0], conf['mus'][1], conf['mus'][2], conf['mus'][3], lamda, conf['gamma'], fh, ft, iters)
    print(cmd)
    os.system(cmd)

# pareto
resFolder = './results/lossVsTime'
iters = 100
lambdas = [1, 1.5, 2]
npkts = 10000
buffers = np.arange(1, 65, 5)
confNames = ['4e', '4f']
muDists = ['EXP', 'CONST']

for confName, lamda,  buffer, muDist in itertools.product(confNames, lambdas, buffers, muDists):
    ft = '_{}.dat'.format(confName)
    conf = confs4[confName]
    fh = resFolder + '/{}_'.format(muDist)
    cmd = './bin/main {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} 0 0 1 0 1'.format(
        scenario, npkts, muDist, buffer, conf['alphas'][0], conf['alphas'][1], conf['alphas'][2], conf['alphas'][3],
        conf['mus'][0], conf['mus'][1], conf['mus'][2], conf['mus'][3], lamda, conf['gamma'], fh, ft, iters)
    print(cmd)
    os.system(cmd)

# timeBoxplots
resFolder = './results/timeBoxplots'
iters = 1
lambdas = [1, 1.5, 2]
npkts = 200000
buffers = [0, 4, 8, 16, 32]  # np.arange(1, 65, 5)  #
confNames = ['4e', '4f']
muDists = ['EXP', 'CONST']

for confName, lamda,  buffer, muDist in itertools.product(confNames, lambdas, buffers, muDists):
    ft = '_{}.dat'.format(confName)
    conf = confs4[confName]
    fh = resFolder + '/{}_'.format(muDist)
    cmd = './bin/main {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} 1 0 0 0 0'.format(
        scenario, npkts, muDist, buffer, conf['alphas'][0], conf['alphas'][1], conf['alphas'][2], conf['alphas'][3],
        conf['mus'][0], conf['mus'][1], conf['mus'][2], conf['mus'][3], lamda, conf['gamma'], fh, ft, iters)
    print(cmd)
    os.system(cmd)

# lossBoxplots
resFolder = './results/lossBoxplots'
iters = 1000
lambdas = [1, 1.5, 2]
npkts = 10000
buffers = [0, 4, 8, 16, 32]  # np.arange(1, 65, 5)  #
confNames = ['4e', '4f']
muDists = ['EXP', 'CONST']

for confName, lamda,  buffer, muDist in itertools.product(confNames, lambdas, buffers, muDists):
    ft = '_{}.dat'.format(confName)
    conf = confs4[confName]
    fh = resFolder + '/{}_'.format(muDist)
    cmd = './bin/main {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} {} 0 0 1 0 0'.format(
        scenario, npkts, muDist, buffer, conf['alphas'][0], conf['alphas'][1], conf['alphas'][2], conf['alphas'][3],
        conf['mus'][0], conf['mus'][1], conf['mus'][2], conf['mus'][3], lamda, conf['gamma'], fh, ft, iters)
    print(cmd)
    os.system(cmd)
