using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(CharacterController))] // enforces dependency on character controller
public class PlayerMove : MonoBehaviour
{
    [SerializeField] Transform target;  // camera

    // sensitivities
    public float moveSpeed = 6.0f;

    // dash variables
    public float dashSpeed;
    public float dashTime;
    private float dashCooldown = 0.5f;
    private bool canDash = true;
    public bool isDashing = false;
    public AudioSource dashAudio;
    public AudioClip dashClip;

    private TrailRenderer tr;

    private Vector3 movement;
    private CharacterController charController;

    // animator
    Animator animator;

    // Start is called before the first frame update
    void Start()
    {
        moveSpeed = PlayerPrefs.GetFloat("PlayerMoveSpeed", moveSpeed);

        // get character controller component
        charController = GetComponent<CharacterController>();

        tr = GetComponent<TrailRenderer>();
        tr.emitting = false;

        animator = GetComponent<Animator>();
    }

    // Update is called once per frame
    void Update()
    {
        // start with zero and add movement components progressively
        movement = Vector3.zero;

        // changes based on horizontal and vertical input
        float horInput = Input.GetAxis("Horizontal");
        float vertInput = Input.GetAxis("Vertical");

        // if input is not zero
        if (horInput != 0 || vertInput != 0)
        {
            animator.SetBool("isWalking", true);

            // x z movement transformed relative to target
            Vector3 right = target.right;
            Vector3 forward = Vector3.Cross(right, Vector3.up);

            // calculate movement
            movement = (right * horInput) + (forward * vertInput);

            // scale by movement speed
            movement *= moveSpeed;
            movement = Vector3.ClampMagnitude(movement, moveSpeed);

            // face movement direction
            //transform.rotation = Quaternion.LookRotation(movement);
            Quaternion direction = Quaternion.LookRotation(movement);
            transform.rotation = Quaternion.Lerp(transform.rotation,
                                    direction, 0);
        }
        else
        {
            animator.SetBool("isWalking", false);
        }

        movement *= Time.deltaTime;

        // Check if the character is grounded
        bool isGrounded = charController.isGrounded;

        if (!isGrounded)
        {
            // Apply gravity
            movement.y -= 9.81f * Time.deltaTime;
        }

        // Calculate dot products to track horizontal and vertical movement
        float dotProduct = Vector3.Dot(movement.normalized, transform.forward);
        float dotProductRight = Vector3.Dot(movement.normalized, transform.right);

        animator.SetFloat("Vertical", dotProduct);
        animator.SetFloat("Horizontal", dotProductRight);

        //move character
        charController.Move(movement);

        if (Input.GetKeyDown(KeyCode.Space))
        {
            StartCoroutine(Dash());
        }
    }

    private IEnumerator Dash()
    {
        if (!isDashing && canDash)
        {
            isDashing = true;
            canDash = false;

            Debug.Log("Dash");
            float startTime = Time.time;
            tr.emitting = true;
            while (Time.time < startTime + dashTime)
            {
                charController.Move(movement * dashSpeed * Time.deltaTime);

                yield return null;
            }
            tr.emitting = false;

            dashAudio.PlayOneShot(dashClip);
            // Wait for cooldown
            yield return new WaitForSeconds(dashCooldown);

            canDash = true;
            isDashing = false;
        }
    }
}
