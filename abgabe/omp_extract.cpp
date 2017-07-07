void OmpSimulator::addForces() {

X\textcolor{orange}{\#pragma}X omp parallel for
    for (int k = 0; k < this->num_bodies; ++k) {
        this->bodies[k].resetForce();
    }

X\textcolor{orange}{\#pragma}X omp parallel for collapse(2)
    for (int i = 0; i < this->num_bodies; i++) {
        for (int j = 0; j < this->num_bodies; j++) {
            if (i != j) {
                this->bodies[i].addForce(this->bodies[j]);
            }
        }
    }

X\textcolor{orange}{\#pragma}X omp parallel for
    for (int i = 0; i < this->num_bodies; i++) {
        this->bodies[i].update(UPDATE_STEP);
    }

}