
#include <iostream>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

float func(float arg) { return arg * arg; }

float pre_intergrete(float a, float b) {
  float h = (b - a);
  float y = (func(a) + func(b)) / 2;
  y = y * h;
  return y;
}

float intergrete(int number, float a, float b, int n) {

  float h = (b - a) / n;
  float second_a = a + number * h;
  float second_b = second_a + h;
  return result;
}

int main(int argc, char **argv) {
  MPI_Status status;
  MPI_Init(NULL, NULL);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  float a = 0;
  float b = 3;
  int n = 10;
  float all = 0;
  float data = 0;

  if (rank == 0) {
    int count = 0;
    int num;
    num = 0;
    for (int i = 1; i < size; i++) {
      MPI_Send(&num, 1, MPI_INT, i, 99, MPI_COMM_WORLD);
      printf("send num %d to %d\n", num, i);
      count = count + 1;
      num = num + 1;
    }
    while (count > 0) {
      int receive_count = 0;
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      MPI_Get_count(&status, MPI_CHAR, &receive_count);
      MPI_Recv(&data, receive_count, MPI_FLOAT, MPI_ANY_SOURCE, MPI_ANY_TAG,
               MPI_COMM_WORLD, &status);
      printf("get process :%d num :%d data :%4.2f\n", status.MPI_SOURCE, num,
             data);
      all = all + data;
      count = count - 1;

      if (num < n) {
        MPI_Send(&num, 1, MPI_INT, status.MPI_SOURCE, 99, MPI_COMM_WORLD);
        printf("send num %d to %d\n", num, status.MPI_SOURCE);
        count = count + 1;
        num = num + 1;
      } else {
        MPI_Send(&num, 1, MPI_INT, status.MPI_SOURCE, 10, MPI_COMM_WORLD);
        printf("send end to %d\n", status.MPI_SOURCE);
      }
      printf("the all = %4.2f\n", all);
    }
  } else {
    int num;
    int receive_count = 0;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_CHAR, &receive_count);
    MPI_Recv(&num, receive_count, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD,
             &status);
    while (status.MPI_TAG != 10) {
      float data = intergrete(num, a, b, n);
      MPI_Send(&data, 1, MPI_FLOAT, 0, 11, MPI_COMM_WORLD);
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

      MPI_Recv(&num, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
  }

  MPI_Finalize();
}
