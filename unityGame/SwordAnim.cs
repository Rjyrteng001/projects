using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwordAnim : MonoBehaviour
{
    Animator animator;

    public WeaponController weapon;

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
        animator.SetBool("isMelee", false);
        if (Input.GetMouseButtonDown(0))
        {
            if (weapon.CanAttack)
            {
                Debug.Log("Melee");
                animator.SetBool("isMelee", true);
            }
        }
    }
}
