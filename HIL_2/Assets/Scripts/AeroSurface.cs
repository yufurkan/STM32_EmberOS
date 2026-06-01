using UnityEngine;

public class AeroSurface : MonoBehaviour
{
    public AeroConfig config;

    [Header("Geometry")]
    public float chord = 0.2f;
    public float span = 1.0f;

    [HideInInspector]
    public float flapAngle = 0f; // Radian input from HIL

    public Vector3 CalculateForces(Vector3 velocity, Vector3 angularVelocity, Vector3 com)
    {
        if (config == null) return Vector3.zero;

        Vector3 relativePos = transform.position - com;
        Vector3 localVel = transform.InverseTransformDirection(velocity + Vector3.Cross(angularVelocity, relativePos));

        if (localVel.sqrMagnitude < 0.1f) return Vector3.zero;

        // Angle of Attack (AoA) + Flap deflection in radians
        float aoa = Mathf.Atan2(-localVel.y, localVel.z);
        aoa += flapAngle;

        float aoaDeg = aoa * Mathf.Rad2Deg;
        float liftCoeff = 0f;
        float dragCoeff = config.skinFriction;

        // Aerodynamic calculations with Stall logic
        if (aoaDeg < config.stallAngleHigh && aoaDeg > config.stallAngleLow)
        {
            // Normal flight envelope
            liftCoeff = config.liftSlope * (aoa - (config.zeroLiftAoA * Mathf.Deg2Rad));
            dragCoeff += Mathf.Abs(liftCoeff) * 0.05f; // Induced drag
        }
        else
        {
            // Post-stall conditions
            float maxLift = config.liftSlope * (config.stallAngleHigh * Mathf.Deg2Rad - (config.zeroLiftAoA * Mathf.Deg2Rad));
            liftCoeff = Mathf.Sign(aoa) * maxLift * config.postStallLiftRatio;

            // Extreme drag penalty during stall
            dragCoeff += 0.3f;
        }

        // Dynamic pressure (0.5 * rho * V^2)
        float dynamicPressure = 0.5f * 1.225f * localVel.sqrMagnitude;
        float area = chord * span;

        Vector3 liftDir = Vector3.Cross(localVel.normalized, Vector3.right).normalized;
        Vector3 dragDir = -localVel.normalized;

        Vector3 liftForce = liftDir * liftCoeff * dynamicPressure * area;
        Vector3 dragForce = dragDir * dragCoeff * dynamicPressure * area;

        return transform.TransformDirection(liftForce + dragForce);
    }

    public void SetFlapAngle(float angleInRadians)
    {
        flapAngle = angleInRadians;
    }
}