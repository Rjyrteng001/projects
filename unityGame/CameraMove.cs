using UnityEngine;

public class CameraMove : MonoBehaviour
{
    public Transform target; // The target to follow (e.g., the player's transform)
    public float followSpeed = 5.0f; // Speed at which the camera follows

    private Vector3 initialOffset; // Store the initial offset

    private void Start()
    {
        // Calculate the initial offset based on the distance between the camera and the target
        initialOffset = transform.position - target.position;
    }

    private void Update()
    {
        // Calculate the desired position based on target's position and initial offset
        Vector3 desiredPosition = target.position + initialOffset;

        // Smoothly move the camera towards the desired position
        transform.position = Vector3.Lerp(transform.position, desiredPosition, followSpeed * Time.deltaTime);
    }
}
