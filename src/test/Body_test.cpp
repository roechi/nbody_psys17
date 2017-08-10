//
// Created by Richard Remus on 04.08.17.
//

#include <gtest/gtest.h>
#include "Body_test.h"
#include <math.h>

TEST(BodyTests, shouldResetForces) {
    Body *body = createTestBodyWithForce(1, 1);
    ASSERT_EQ(body->fx, 1.0);
    ASSERT_EQ(body->fy, 1.0);

    body->resetForce();

    ASSERT_EQ(body->fx, 0.0);
    ASSERT_EQ(body->fy, 0.0);
}

TEST(BodyTests, shouldAddForceHorizontal) {
    Body *body_one = new Body(1, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(1, 1, 0, 0, 0, 0, 0);

    body_one->addForce(*body_two, 1, 0);

    ASSERT_EQ(body_one->fx, 1.0);
    ASSERT_EQ(body_one->fy, 0.0);
}

TEST(BodyTests, shouldAddForceVertical) {
    Body *body_one = new Body(1, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(1, 0, 1, 0, 0, 0, 0);

    body_one->addForce(*body_two, 1, 0);

    ASSERT_EQ(body_one->fy, 1.0);
    ASSERT_EQ(body_one->fx, 0.0);
}

TEST(BodyTests, shouldCalculateDistanceHorizontal) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(0, 1, 0, 0, 0, 0, 0);

    ASSERT_EQ(body_one->distanceTo(*body_two), 1);
}

TEST(BodyTests, shouldCalculateDistanceVertical) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(0, 0, 1, 0, 0, 0, 0);

    ASSERT_EQ(body_one->distanceTo(*body_two), 1);
}

TEST(BodyTests, shouldCalculateDistanceDiagonal) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(0, 1, 1, 0, 0, 0, 0);

    ASSERT_EQ(body_one->distanceTo(*body_two), sqrt(2));
}

TEST(BodyTests, shouldCalculateDistanceDiagonal2) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 0, 0);
    Body *body_two = new Body(0, 3, 5, 0, 0, 0, 0);

    ASSERT_EQ(body_one->distanceTo(*body_two), sqrt(34));
}

TEST(BodyTests, shouldUpdateHorizontal) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 1, 0);

    body_one->update(1);

    ASSERT_EQ(body_one->rx, 1);
}

TEST(BodyTests, shouldUpdateVertical) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 0, 1);

    body_one->update(1);

    ASSERT_EQ(body_one->ry, 1);
}

TEST(BodyTests, shouldUpdateDiagonal) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 1, 1);

    body_one->update(1);

    ASSERT_EQ(body_one->rx, 1);
    ASSERT_EQ(body_one->ry, 1);
}

TEST(BodyTests, shouldUpdateDiagonalMultipleSteps) {
    Body *body_one = new Body(0, 0, 0, 0, 0, 1, 1);

    body_one->update(1);

    ASSERT_EQ(body_one->rx, 1);
    ASSERT_EQ(body_one->ry, 1);

    body_one->update(3);

    ASSERT_EQ(body_one->rx, 13);
    ASSERT_EQ(body_one->ry, 13);
}

Body *createTestBody() { return new Body(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0); }

Body *createTestBodyWithForce(double fx, double fy) {
    return new Body(1.0, 0.0, 0.0, 0.0, 0.0, fx, fy);
}
