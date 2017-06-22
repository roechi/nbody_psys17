kernel void nbody_move(global float* masses,
                       global float* positions,
                       global float* velocities,
                       global float* forces,
                       int num_bodies,
                       float step_size,
                       int WORK_GROUP_SIZE)
{
//    printf("Hello, World!\n");
//    printf("num_bodies=%i, step_size=%f, work_group_size=%i\n",num_bodies,step_size,WORK_GROUP_SIZE);
    for(int n = 0; n < WORK_GROUP_SIZE; n++) {

        // Values of body of interest
        int cur_idx_1d = (get_global_id(0) * WORK_GROUP_SIZE) + n;
        int cur_idx_2d = cur_idx_1d * 2;

        float2 cur_pos = (float2)(positions[cur_idx_2d],positions[cur_idx_2d+1]);
        float cur_mass = masses[cur_idx_1d];
        const float GRAVITATIONAL_CONSTANT = 39.5;

        // Accumulate forces from other bodies
        float2 force = (float2)(0.0, 0.0);

        for(int i = 0; i < num_bodies; i++) {
            int idx_1d = i;
            int idx_2d = i * 2;

            if(idx_1d != cur_idx_1d) {
                float2 pos = (float2)(positions[idx_2d],positions[idx_2d+1]);
                float mass = masses[idx_1d];

                float EPS = 0.00000000001f;      // softening parameter (just to avoid infinities)
                float2 pos_delta = pos - cur_pos;
                float dist = sqrt(pos_delta.x*pos_delta.x + pos_delta.y*pos_delta.y);
                float F = (GRAVITATIONAL_CONSTANT * cur_mass * mass) / (dist*dist + EPS*EPS);

                force += F * pos_delta / dist;
            }
        }

        forces[cur_idx_2d] = force.x;
        forces[cur_idx_2d+1] = force.y;
    }

    for (int j = 0; j < WORK_GROUP_SIZE; ++j) {

        // Values of body of interest
        int cur_idx_1d = (get_global_id(0) * WORK_GROUP_SIZE) + j;
        int cur_idx_2d = cur_idx_1d * 2;
        float cur_mass = masses[cur_idx_1d];
        float2 force = (float2)(forces[cur_idx_2d],forces[cur_idx_2d+1]);

        // Update position
        velocities[cur_idx_2d] += step_size * force.x / cur_mass;
        velocities[cur_idx_2d+1] += step_size * force.y / cur_mass;
        positions[cur_idx_2d] += step_size * velocities[cur_idx_2d];
        positions[cur_idx_2d+1] += step_size * velocities[cur_idx_2d+1];
    }

}