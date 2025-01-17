// Tencent is pleased to support the open source community by making ncnn available.
//
// Copyright (C) 2020 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except
// in compliance with the License. You may obtain a copy of the License at
//
// https://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed
// under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.

#include "layer/receiver.h"
#include "testutil.h"
#if NCNN_MPI
#include <mpi.h>
#include <memory>
#include <iostream>
#endif


static int test_receiver(const ncnn::Mat& a)
{
#if NCNN_MPI

    ncnn::ParamDict pd;
    int total_size = a.total();
    int total_nodes = 1;
    pd.set(0, 1); // dest_id: 1
    pd.set(1, total_size); // send size: 1
    pd.set(2, total_nodes); // send nodes: 1
    
    std::vector<ncnn::Mat> weights(1);
    weights[0] = RandomMat(total_nodes);
    for (int i = 0; i< weights[0].total(); i++){
    weights[0][i] = 1;
    }

    int ret = test_layer<ncnn::Receiver>("Receiver", pd, weights, a);
    if (ret != 0)
    {
        fprintf(stderr, "test_receiver failed a.dims=%d a=(%d %d %d)\n", a.dims, a.w, a.h, a.c);
    }

#endif

    return 0;
}

static int test_receiver_0()
{
    return 0
           || test_receiver(RandomMat(5, 7, 24))
           || test_receiver(RandomMat(7, 9, 12))
           || test_receiver(RandomMat(3, 5, 13));
}

static int test_receiver_1()
{
    return 0
           || test_receiver(RandomMat(15, 24))
           || test_receiver(RandomMat(19, 12))
           || test_receiver(RandomMat(17, 15));
}

static int test_receiver_2()
{
    return 0
           || test_receiver(RandomMat(128))
           || test_receiver(RandomMat(124))
           || test_receiver(RandomMat(127));
}

int main(int argc, char** argv)
{
    SRAND(7767517);

    int numprocs, rank, namelen;                                                                                                                    
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(processor_name, &namelen);
    test_receiver(RandomMat(2));
//    int a = 0
//           || test_receiver_0()
//           || test_receiver_1()
//           || test_receiver_2();
MPI::Finalize();
    return 0;
}
