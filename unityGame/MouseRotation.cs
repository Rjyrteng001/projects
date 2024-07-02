using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MouseRotation : MonoBehaviour
{
    public Camera cam;
    public Transform player; // Player's transform

    private void Update()
    {
        // Get the player's current y position as the desired y origin
        float yOrigin = player.position.y;

        // Calculate the intersection point on the XZ plane using the player's y origin
        Ray ray = cam.ScreenPointToRay(Input.mousePosition);
        float distanceToPlane = (yOrigin - ray.origin.y) / ray.direction.y;
        Vector3 intersectionPoint = ray.GetPoint(distanceToPlane);

        // Calculate the direction the player is looking at
        Vector3 lookDirection = intersectionPoint - player.position;

        // Rotate the player to face the look direction
        if (lookDirection != Vector3.zero)
        {
            player.rotation = Quaternion.LookRotation(lookDirection, Vector3.up);
        }
    }
}
