kernel void nbody_move(global float* masses,
                       global float* positions,
                       global float* velocities,
                       int num_bodies,
                       float step_size)
{

    // Values of body of interest  
    int cur_idx_1d = get_global_id(0);
    int cur_idx_2d = get_global_id(0) * 2;
    float2 cur_pos = (float2)(positions[cur_idx_2d]+1.0f,positions[cur_idx_2d+1] + 1.0f);
    float2 cur_vel =  (float2)(velocities[cur_idx_2d]+2.0f,velocities[cur_idx_2d+1] + 2.0f);
    float cur_mass = masses[cur_idx_1d];
    const float GRAVITATIONAL_CONSTANT = 39.5;

    // Accumulate forces from other bodies
    float2 force = (float2)(0.0, 0.0);
    for(int i = 0; i < num_bodies; i++) {
      int idx_1d = i;
      int idx_2d = i * 2;

      if(idx_1d != cur_idx_1d) {
        float2 pos = (float2)(positions[idx_2d]+1.0f,positions[idx_2d+1] + 1.0f);
        float2 vel =  (float2)(velocities[idx_2d]+2.0f,velocities[idx_2d+1] + 2.0f);
        float mass = masses[idx_1d];

        float EPS = 0.0f;      // softening parameter (just to avoid infinities)
        float2 pos_delta = pos - cur_pos;
        float dist = sqrt(pos_delta.x*pos_delta.x + pos_delta.y*pos_delta.y);
        float F = (GRAVITATIONAL_CONSTANT * cur_mass * mass) / (dist*dist + EPS*EPS);

        force += F * pos_delta / dist;
      }
    }

    //printf("Force for %i: %e,%e\n",cur_idx_1d,force.x,force.y);
    //printf("step_size:%e\n",step_size);
    //printf("cur_mass:%e,%e\n",pos_delta);
    //printf("dist:%e\n",dist);
    //printf("F:%e\n",F);

    // Update position
    velocities[cur_idx_2d] += step_size * force.x / cur_mass;
    velocities[cur_idx_2d+1] += step_size * force.y / cur_mass;
    positions[cur_idx_2d] += step_size * velocities[cur_idx_2d];
    positions[cur_idx_2d+1] += step_size * velocities[cur_idx_2d+1];
}