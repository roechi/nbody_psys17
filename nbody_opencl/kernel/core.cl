void kernel simple_add(global const float* A, global const float* B, global float* C){
       size_t idx = get_global_id(0);
       C[idx]=A[idx]+B[idx];
}