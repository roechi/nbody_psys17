__kernel void add_vec( __global float *dest,                     
                       __global float *a,                     
                       __global float *b,                     
                       const size_t size)                  
{                                                                                        
    int id = get_global_id(0);                                                                            
    dest[id] = a[id] + b[id];  
}                                                             
                                                               