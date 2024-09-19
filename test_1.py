import matplotlib.pyplot as plt


trace_data = {
    "bzip.trace": {
        "rand": {
            1: 0.6297,
            10: 0.0173,
            100: 0.0012,
            1000: 0.0003,
            10000: 0.0003,
            100000: 0.0003,
            1000000: 0.0003
        },
        "lru": {
            1: 0.6297,
            10: 0.0171,
            100: 0.0009,
            1000: 0.0003,
            10000: 0.0003,
            100000: 0.0003,
            1000000: 0.0003
        },
        "clock": {
            1: 0.6297,
            10: 0.0292,
            100: 0.0009,
            1000: 0.0003,
            10000: 0.0003,
            100000: 0.0003,
            1000000: 0.0003
        }
    },
    "gcc.trace": {
        "rand": {
            1: 0.7161,
            10: 0.1925,
            100: 0.0592,
            1000: 0.0073,
            10000: 0.0029,
            100000: 0.0029,
            1000000: 0.0029
        },
        "lru": {
            1: 0.7161,
            10: 0.1507,
            100: 0.0457,
            1000: 0.0045,
            10000: 0.0029,
            100000: 0.0029,
            1000000: 0.0029
        },
        "clock": {
            1: 0.7161,
            10: 0.1587,
            100: 0.0476,
            1000: 0.0049,
            10000: 0.0029,
            100000: 0.0029,
            1000000: 0.0029
        }
    },
    "sixpack.trace": {
        "rand": {
            1: 0.7924,
            10: 0.2004,
            100: 0.0358,
            1000: 0.0059,
            10000: 0.0039,
            100000: 0.0039,
            1000000: 0.0039
        },
        "lru": {
            1: 0.7924,
            10: 0.1545,  # Only partial value provided, please check this value in your trace.
            100: 0.0396,
            1000: 0.0051,
            10000: 0.0039,
            100000: 0.0039,
            1000000: 0.0039
        },
        "clock": {
            1: 0.7924,
            10: 0.1645,  # Only partial value provided, please check this value in your trace.
            100: 0.0401,
            1000: 0.0052,
            10000: 0.0039,
            100000: 0.0039,
            1000000: 0.0039
        }
    }
}



L = {"a": [1,2,3,4,5,6,7]}

