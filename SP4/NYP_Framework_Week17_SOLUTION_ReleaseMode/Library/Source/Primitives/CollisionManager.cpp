#include "CollisionManager.h"

#include <algorithm>
#include <iostream>
using namespace std;

/**
 @brief Sphere-Sphere Collision Test
 */
bool CCollisionManager::SphereSphereCollision(  const glm::vec3 sphereCentreA, 
                                                const float sphereRadiusA,
                                                const glm::vec3 sphereCentreB, 
                                                const float sphereRadiusB)
{
    return (glm::distance(sphereCentreA, sphereCentreB) <= sphereRadiusA + sphereRadiusA);
}

/**
 @brief Box-Box Collision Test
 */
bool CCollisionManager::BoxBoxCollision(    const glm::vec3 aMin, 
                                            const glm::vec3 aMax,
                                            const glm::vec3 bMin, 
                                            const glm::vec3 bMax)
{
    // true == collision, false == no collision
    return !(   aMax.x < bMin.x || aMax.z < bMin.z ||
                aMax.y < bMin.y || aMax.y < bMin.y ||
                aMin.x > bMax.x || aMin.z > bMax.z      );
}

/**
 @brief Ray-Sphere Collision Test
 */
bool CCollisionManager::RaySphereCollision( const glm::vec3 sphereCentre, 
                                            const float sphereRadius,
                                            const glm::vec3 rayOrigin, 
                                            const glm::vec3 rayDirection,
                                            float& t)
{
    glm::vec3 oc = rayOrigin - sphereCentre;
    float a = glm::dot(rayDirection, rayDirection);
    float b = 2.0f * dot(oc, rayDirection);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4 * a * c;

    // If discriminant < 0, the line of the ray does not intersect the sphere(missed);
    if (discriminant < 0.0)
    {
        return false;
    }
    else {
        // If discriminant = 0, the line of the ray just touches the sphere in one point(tangent);
        // If discriminant > 0, the line of the ray touches the sphere in two points(intersected).
        // If both t are positive, ray is facing the sphereand intersecting.
        // If one t positive, one t negative, ray is shooting from inside.
        // If both t are negative, ray is shooting away from the sphere, which is technically impossible.

        // Check for the negative/smaller result of the sqrt
        float numerator = -b - sqrt(discriminant);
        if (numerator > 0.0)
        {
            // Calculate the parameter, t
            t = numerator / (2.0f * a);
            return true;
        }

        // Check for the positive/larger result of the sqrt
        numerator = -b + sqrt(discriminant);
        if (numerator > 0.0)
        {
            // Calculate the parameter, t
            t = numerator / (2.0f * a);
            return true;
        }
        else
        {
            return false;
        }
    }

    /*
    // If you wish to calculate the hit point of this collision
    glm::vec3 hitPoint = rayOrigin + t * rayDirection;
    */

    return true;
}

/**
 @brief Ray-Box Collision Test
 */
bool CCollisionManager::RayBoxCollision(const glm::vec3 boxMin, 
                                        const glm::vec3 boxMax,
                                        const glm::vec3 rayOrigin, 
                                        const glm::vec3 rayDirection,
                                        float& t)
{
    // Calculate the parameter t when the ray is projected onto the x-axis
    float txmin = (boxMin.x - rayOrigin.x) / rayDirection.x;
    float txmax = (boxMax.x - rayOrigin.x) / rayDirection.x;

    // Swap if the 2 parameters if needed
    if (txmin > txmax) swap(txmin, txmax);

    // Calculate the parameter t when the ray is projected onto the y-axis
    float tymin = (boxMin.y - rayOrigin.y) / rayDirection.y;
    float tymax = (boxMax.y - rayOrigin.y) / rayDirection.y;

    // Swap if the 2 parameters if needed
    if (tymin > tymax) swap(tymin, tymax);

    // The ray misses the box in the x-y plane when txmin is greater 
    // than tymax or when tymin is greater than tymin
    if ((txmin > tymax) || (tymin > txmax))
        return false;

    // Find the largest and smallest values for
    // the parameter t for x-axis vs y-axis
    txmin = std::max(tymin, txmin);
    txmax = std::min(tymax, txmax);

    // Calculate the parameter t when the ray is projected onto the z-axis
    float tzmin = (boxMin.z - rayOrigin.z) / rayDirection.z;
    float tzmax = (boxMax.z - rayOrigin.z) / rayDirection.z;

    // Swap if the 2 parameters if needed
    if (tzmin > tzmax) swap(tzmin, tzmax);

    // The ray misses the box in the x-z plane when txmin is greater 
    // than tzmax or when tzmin is greater than tymin
    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    // Find the largest and smallest values for
    // the parameter t for x-axis vs z-axis
    txmin = std::max(tzmin, txmin);
    txmax = std::min(tzmax, txmax);

    // If tFar is negative, then check if tNear is negative too, otherwise return tNear.
    if (txmax < 0) {
        if (txmin < 0)
        {
            return false;
        }
        t = txmax;
    }
    else
    {
        t = txmin;
    }

    return true;
}

/**
 @brief Ray-Box Collision Test : Overloaded
 */
bool CCollisionManager::RayBoxCollision(const glm::vec3 boxMin, 
                                        const glm::vec3 boxMax,
                                        const glm::vec3 rayStart,
                                        const glm::vec3 rayEnd)
{
    glm::vec3 ray = rayEnd - rayStart;
    glm::vec3 rayDirection = glm::normalize(ray);

    // Calculate the parameter t when the ray is projected onto the x-axis
    float txmin = (boxMin.x - rayStart.x) / rayDirection.x;
    float txmax = (boxMax.x - rayStart.x) / rayDirection.x;

    // Swap if the 2 parameters if needed
    if (txmin > txmax) swap(txmin, txmax);

    // Calculate the parameter t when the ray is projected onto the y-axis
    float tymin = (boxMin.y - rayStart.y) / rayDirection.y;
    float tymax = (boxMax.y - rayStart.y) / rayDirection.y;

    // Swap if the 2 parameters if needed
    if (tymin > tymax) swap(tymin, tymax);

    // The ray misses the box in the x-y plane when txmin is greater 
    // than tymax or when tymin is greater than tymin
    if ((txmin > tymax) || (tymin > txmax))
        return false;

    // Find the largest and smallest values for
    // the parameter t for x-axis vs y-axis
    txmin = std::max(tymin, txmin);
    txmax = std::min(tymax, txmax);

    // Calculate the parameter t when the ray is projected onto the z-axis
    float tzmin = (boxMin.z - rayStart.z) / rayDirection.z;
    float tzmax = (boxMax.z - rayStart.z) / rayDirection.z;

    // Swap if the 2 parameters if needed
    if (tzmin > tzmax) swap(tzmin, tzmax);

    // The ray misses the box in the x-z plane when txmin is greater 
    // than tzmax or when tzmin is greater than tymin
    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    // Find the largest and smallest values for
    // the parameter t for x-axis vs z-axis
    txmin = std::max(tzmin, txmin);
    txmax = std::min(tzmax, txmax);

    // Check if the collision point lies between rayStart and rayEnd
    if (glm::length((txmin * rayDirection)) < glm::length(ray))
    {
        return true;
    }

    // Check if the collision point lies between rayStart and rayEnd
    if (glm::length((txmax * rayDirection)) < glm::length(ray))
    {
        return true;
    }

    return false;
}
