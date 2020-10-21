import numpy as np
import gzip


def load_data(train_x, train_y, test_x, test_y, pred_x):
    print("Loading data from files. Please wait...")

    # Training data
    tr_x = np.loadtxt(gzip.open(train_x, "rb"), delimiter=",", dtype=np.float128)
    tr_y = [parse_result(y)
            for y in np.loadtxt(gzip.open(train_y, "rb"), dtype=np.float128)]

    # Testing data
    t_x = np.loadtxt(gzip.open(test_x, "rb"), delimiter=",", dtype=np.float128)
    t_y = [parse_result(y)
           for y in np.loadtxt(gzip.open(test_y, "rb"), dtype=np.float128)]

    # Test input 2 used for prediction, if it is the same as test_x don't load
    # from file
    if test_x == pred_x:
        p_x = t_x
    else:
        p_x = np.loadtxt(gzip.open(pred_x, "rb"), delimiter=",", dtype=np.float128)

    return tr_x, tr_y, t_x, t_y, p_x


def parse_result(digit):
    """
    Creates a 10-sized vector (0-9) with all values
    as 0 except the index of digit, which = 1
    """
    r = np.zeros(10)
    r[int(digit)] = 1.0
    return r
