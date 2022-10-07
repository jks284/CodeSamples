// CodeSample-2D_Line_of_Sight.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <math.h>

const float PI_F = 3.14159f;
const float MAX_FLOAT_TOLERANCE = 0.0001f;

/// <summary>
/// A two dimensional vector with implementations of some simple vector math methods.
/// </summary>
class Vec2D
{
public:
    Vec2D(float x = 0.0f, float y = 0.0f) :
        m_x(x),
        m_y(y)
    { }

    /// <summary>
    /// Calculates the dot product between this vector and vecIn
    /// </summary>
    /// <param name="vecIn"></param>
    /// <returns></returns>
    float DotProduct(const Vec2D& vecIn) const;

    float GetMagnitude() const;
    Vec2D GetNormalized() const;
    float GetX() const { return m_x; }
    float GetY() const { return m_y; }
    void SetX(float x) { m_x = x; }
    void SetY(float y) { m_y = y; }

    /// <summary>
    /// Determines whether two vectors are equivalent.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    bool operator==(const Vec2D& rhs) const
    {
        return MAX_FLOAT_TOLERANCE > std::abs(m_x - rhs.GetX()) &&
            MAX_FLOAT_TOLERANCE > std::abs(m_y - rhs.GetY());
    }
    /// <summary>
    /// Performs vector addition between two vectors.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Vec2D operator+(const Vec2D& rhs) const
    {
        return Vec2D(m_x + rhs.GetX(), m_y + rhs.GetY());
    }

    /// <summary>
    /// Performs vector subtraction between two vectors.
    /// </summary>
    /// <param name="rhs"></param>
    /// <returns></returns>
    Vec2D operator-(const Vec2D& rhs) const
    {
        return Vec2D(m_x - rhs.GetX(), m_y - rhs.GetY());
    }

private:
    float m_x;
    float m_y;
};

float Vec2D::DotProduct(const Vec2D& vecIn) const
{
    float xComp = m_x * vecIn.GetX();
    float yComp = m_y * vecIn.GetY();
    return xComp + yComp;
};

float Vec2D::GetMagnitude() const
{
    return sqrtf( (m_x*m_x) + (m_y*m_y) );
}

Vec2D Vec2D::GetNormalized() const
{
    Vec2D result;
    float magnitude = GetMagnitude();

    //don't divide by 0
    if (magnitude != 0)
    {
        result = Vec2D(m_x / magnitude, m_y / magnitude);
    }

    return result;
}

/// <summary>
/// A simple camera that can tell whether a location is within the camera's sight line.
/// </summary>
class Camera2D
{
public:
    Camera2D(Vec2D position = Vec2D(0.0f, 0.0f),
        Vec2D orientation = Vec2D(0.0f, 1.0f),
        float fieldOfView = PI_F,
        float viewDistance = std::numeric_limits<float>::max()
    ) :
        m_position(position),
        m_fieldOfView(std::abs(fieldOfView)),
        m_viewDistance(std::abs(viewDistance))
    { 
        SetOrientation(orientation);
    }

    /// <summary>
    /// Determines whether the target position is within sight of the camera object.
    /// </summary>
    /// <param name="targetPosition"></param>
    /// <returns></returns>
    bool CanSeeTarget(const Vec2D& targetPosition) const;

    void SetPosition(Vec2D position) { m_position = position; }

    //enforce orientation vector to be a unit vector
    void SetOrientation(Vec2D orientation) { m_orientation = orientation.GetNormalized(); }

    /// <summary>
    /// Sets the field of view in radians.
    /// </summary>
    /// <param name="angle"></param>
    void SetFieldOfView(float angleRad) { m_fieldOfView = angleRad; }
    void SetViewDistance(float distance) { m_viewDistance = distance; }

private:
    Vec2D m_position;
    Vec2D m_orientation;
    float m_fieldOfView;
    float m_viewDistance;
};

bool Camera2D::CanSeeTarget(const Vec2D& targetPosition) const
{
    //Camera can't see something on it's own position
    if (targetPosition == m_position) return false;

    bool targetVisible = false;

    //get vector/distance to target
    Vec2D vectorToTarget = targetPosition - m_position;
    float distanceToTarget = vectorToTarget.GetMagnitude();

    //check if target is in range of sight
    if (distanceToTarget <= m_viewDistance)
    {
        float dot = m_orientation.DotProduct(vectorToTarget);
        //find angle between target and orientation
        //NOTE: orientation is a unit vector, otherwise its magnitude would be in the denominator
        float angleToTarget = std::acos(dot / distanceToTarget);
        //if the angle is within field of view, the camera can see the target
        targetVisible = angleToTarget < m_fieldOfView / 2.0f;
    }
    return targetVisible;
}

int main()
{
    std::cout << "----- Vec2D Tests -----" << std::endl << std::endl;

    //origin
    Vec2D vec0(0.0f, 0.0f);
    //unit right
    Vec2D vec1(1.0f, 0.0f);
    //unit up
    Vec2D vec2(0.0f, 1.0f);
    //unit left
    Vec2D vec3(-1.0f, 0.0f);
    //unit down
    Vec2D vec4(0.0f, -1.0f);

    //test DotProduct
    std::cout << "Parallel unit vectors in the same direction have a dot product of " << vec1.DotProduct(vec1) << ". Expected 1" << std::endl;
    std::cout << "Orthogonal unit vectors have a dot product of " << vec1.DotProduct(vec2) << ". Expected 0" << std::endl;
    std::cout << "Parallel unit vectors in opposite directions have a dot product of " << vec1.DotProduct(vec3) << ". Expected -1" << std::endl;
    std::cout << std::endl;

    Vec2D vec5(5.0f, 5.0f);
    Vec2D vec6(42.0f, -12.0f);
    std::cout << "(5,5) dot (5,5) is " << vec5.DotProduct(vec5) << ". Expected 50" << std::endl;
    std::cout << "(5,5) dot (42,-12) is " << vec5.DotProduct(vec6) << ". Expected 150" << std::endl;
    std::cout << std::endl;

    //test GetMagnitude
    std::cout << "Magnitude of (0,0) is " << vec0.GetMagnitude() << ". Expected 0" << std::endl;
    std::cout << "Magnitude of (1,0) is " << vec1.GetMagnitude() << ". Expected 1" << std::endl;
    std::cout << "Magnitude of (5,5) is " << vec5.GetMagnitude() << ". Expected ~7.071" << std::endl;
    std::cout << "Magnitude of (42,-12) is " << vec6.GetMagnitude() << ". Expected ~43.681" << std::endl;
    std::cout << std::endl;

    //test GetNormalized
    Vec2D vec1Norm = vec1.GetNormalized();
    Vec2D vec5Norm = vec5.GetNormalized();
    Vec2D vec6Norm = vec6.GetNormalized();
    std::cout << "Normalized vector of (1,0) is (" << vec1Norm.GetX() << "," << vec1Norm.GetY() << "). Expected (1,0)" << std::endl;
    std::cout << "Normalized vector of (5,5) is (" << vec5Norm.GetX() << "," << vec5Norm.GetY() << "). Expected (~0.7071,~0.7071)" << std::endl;
    std::cout << "Normalized vector of (42,-12) is (" << vec6Norm.GetX() << "," << vec6Norm.GetY() << "). Expected (~0.962,~-0.275)" << std::endl;
    std::cout << std::endl;

    //test operators
    Vec2D vec5MinusVec6 = vec5 - vec6;
    Vec2D vec5PlusVec6 = vec5 + vec6;
    Vec2D vec16(1.01f, 0.0f);
    std::cout << "(5,5) - (42,-12) is (" << vec5MinusVec6.GetX() << "," << vec5MinusVec6.GetY() << "). Expected (-37,17)" << std::endl;
    std::cout << "(5,5) + (42,-12) is (" << vec5PlusVec6.GetX() << "," << vec5PlusVec6.GetY() << "). Expected (47,-7)" << std::endl;
    std::cout << "(1,0) == (1,0) is true. Actual result = " << (vec1 == vec1) << std::endl;
    std::cout << "(1,0) == (1.01,0) is false. Actual result = " << (vec1 == vec16) << std::endl;
    std::cout << std::endl;

    std::cout << "----- Camera Tests -----" << std::endl << std::endl;

    Vec2D vec7(0.0f, 300.0f);
    Vec2D vec8(0.0f, 50.0f);
    Vec2D vec9(0.0f, -50.0f);
    Vec2D vec10(50.0f, 0.0f);
    Vec2D vec11(-50.0f, 0.0f);
    Vec2D vec12(50.0f, 50.0f);
    Vec2D vec13(50.0f, 50.1f);
    Vec2D vec14(-50.0f, 50.0f);
    Vec2D vec15(-50.0f, 50.1f);
    //origin position, pointed up, 90deg field of view, 100 view distance
    Camera2D cam0(Vec2D(0.0f, 0.0f), Vec2D(0.0f, 1.0f), PI_F / 2.0f, 100.0f);

    //test CanSeeTarget
    std::cout << "100 unit view distance camera shouldn't see something 300 units away. Actual result = " << cam0.CanSeeTarget(vec7) << std::endl;
    std::cout << "100 unit view distance camera should see something 50 units away. Actual result = " << cam0.CanSeeTarget(vec8) << std::endl;
    std::cout << "Camera shouldn't see something in range but behind it. Actual result = " << cam0.CanSeeTarget(vec9) << std::endl;
    std::cout << "Camera shouldn't see something in range but to the right. Actual result = " << cam0.CanSeeTarget(vec10) << std::endl;
    std::cout << "Camera shouldn't see something in range but to the left. Actual result = " << cam0.CanSeeTarget(vec11) << std::endl;
    std::cout << "Camera shouldn't see something on edge of vision. Actual result = " << cam0.CanSeeTarget(vec12) << std::endl;
    std::cout << "Camera should see something just within edge of vision. Actual result = " << cam0.CanSeeTarget(vec13) << std::endl;
    std::cout << "Camera shouldn't see something on edge of vision (opposite side). Actual result = " << cam0.CanSeeTarget(vec14) << std::endl;
    std::cout << "Camera should see something just within edge of vision (opposite side). Actual result = " << cam0.CanSeeTarget(vec15) << std::endl;
}