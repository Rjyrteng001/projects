using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class gunAnim : MonoBehaviour
{
    Animator animator;

    public GunController fireball;
    private float fireballDelay = 0.4f;

    private void Start()
    {

        GameObject playerObject = GameObject.Find("PlayerMesh");
        if (playerObject != null)
        {
            // Get the Animator component from the player object
            animator = playerObject.GetComponent<Animator>();
        }
        else
        {
            Debug.LogError("Player root object not found.");
        }
    }

    // Update is called once per frame
    void Update()
    {
        animator.SetBool("isShoot", false);
        if (Input.GetMouseButtonDown(1))
        {
            if (fireball.readyToShoot && fireball.playerMana > 10.0f)
            {
                animator.SetBool("isShoot", true);
                Invoke(nameof(resetAnimShot), fireballDelay);
            }
        }
    }

    void resetAnimShot()
    {
        animator.SetBool("isShoot", false);
    }
}
