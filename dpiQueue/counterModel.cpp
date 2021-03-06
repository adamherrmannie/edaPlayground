// Simple Counter Model with DPI calls
// Author: Adam Herrmann

// 2 counters: Counter 0 counting: 0, 1, 2, 3
//             Counter 1 counting: 0, 2, 4, 6


#include <stdio.h>
#include <iostream>
#include <svdpi.h>
#include <vector>
#include <deque>

using namespace std;

typedef struct {
    int data;
    int info;
}  ModelData;

// Vector of queues to store example results to be compared to the design
vector<deque<int> > blockTapPoint;

extern "C" void sendDataFromUnit(int id, int data);

// Model to generate the data
extern "C" void startModel()
{
  printf("[C Model] Model generating data for 4 blocks\n");
  blockTapPoint.resize(4);
  
  for (int blk = 0; blk < 2; ++blk)
  	for (int i = 0; i < 4; ++i)
    {
    	blockTapPoint[blk].push_back(i*(blk+1));
        sendDataFromUnit(blk, i*(blk+1));
    }
}

extern "C" void structTest(ModelData* data)
{
  printf("[C Model] Struct data before: %d, info: %d\n", data->data, data->info);
  data->data = 5;
  data->info = 4;
  printf("[C Model] Struct data after: %d, info: %d\n", data->data, data->info);
}

// Interface to get the data from the model queues

extern "C" int getDataFromUnit(int id)
{
  //printf("[C Model] Received request from SV world for blk %d\n", id);
  int retVal = 0;
  if (blockTapPoint[id].size())
  {
  	retVal = blockTapPoint[id].front();
  	blockTapPoint[id].pop_front();
  }
  else
  {
    printf("[C Model] ERROR: No more data left in queue\n");
  }
  return retVal;
}

extern "C" int getDataCountFromUnit(int id)
{
  //printf("[C Model] Received request from SV world for blk %d\n", id);
  return blockTapPoint[id].size();
}
