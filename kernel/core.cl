kernel void nbody_move(global float* masses,
                       global float* positions,
                       global float* velocities,
                       global float* forces,
                       int NUM_BODIES,
                       float STEP_SIZE,
                       int WORK_GROUP_SIZE)
{
    const float GRAVITATIONAL_CONSTANT = 39.5f;
    const float EPS = 5.0f;

    // Calculate forces
    for(int n = 0; n < WORK_GROUP_SIZE; n++) {
        int a1d = (get_global_id(0) * WORK_GROUP_SIZE) + n;
        int a2d = a1d * 2;
        float a_rx = positions[a2d];
        float a_ry = positions[a2d+1];
        float fx = 0.0;
        float fy = 0.0;
        for(int i = 0; i < NUM_BODIES; i++) {
            int b1d = i;
            int b2d = i * 2;
            if(b1d != a1d) {
                float b_rx = positions[b2d];
                float b_ry = positions[b2d+1];
                float mass = masses[b1d];
                float dx = b_rx - a_rx;
                float dy = b_ry - a_ry;
                float dist = sqrt(pow(dx,2) + pow(dy,2));
                float F = (mass / pow(dist*dist + EPS*EPS, 1.5));
                fx += F * dx;
                fy += F * dy;
            }
        }
        forces[a2d] = GRAVITATIONAL_CONSTANT * fx;
        forces[a2d+1] = GRAVITATIONAL_CONSTANT * fy;
    }

    // Update positions
    for (int a = 0; a < WORK_GROUP_SIZE; ++a) {
        int a1d = (get_global_id(0) * WORK_GROUP_SIZE) + a;
        int a2d = a1d * 2;
        float fx = forces[a2d];
        float fy = forces[a2d+1];
        velocities[a2d] += STEP_SIZE * fx ;
        velocities[a2d+1] += STEP_SIZE * fy;
        positions[a2d] += STEP_SIZE * velocities[a2d];
        positions[a2d+1] += STEP_SIZE * velocities[a2d+1];
    }

}