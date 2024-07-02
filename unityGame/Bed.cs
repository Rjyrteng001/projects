using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bed : MonoBehaviour
{
    public GameObject upgradeMenu;
    public PlayerMove playerMovement;

    private bool isMenuOpen = false;
    private bool isPlayerInRange = false;

    void Start()
    {
        upgradeMenu.SetActive(isMenuOpen);
        playerMovement = FindObjectOfType<PlayerMove>();
    }

    // Update is called once per frame
    void Update()
    {
        // Check if the player is pressing the 'E' key
        if (Input.GetKeyDown(KeyCode.E) && isPlayerInRange)
        {
            if (!isMenuOpen)
            {
                OpenMenu();
            }
            else
            {
                CloseMenu();
            }
        }
    }

    private void OnTriggerEnter(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            isPlayerInRange = true;
        }
    }

    private void OnTriggerExit(Collider other)
    {
        if (other.CompareTag("Player"))
        {
            isPlayerInRange = false;
            CloseMenu(); // Close the menu when player moves away from the bed
        }
    }

    private void OpenMenu()
    {
        isMenuOpen = true;
        upgradeMenu.SetActive(true);
        Time.timeScale = 0f;
    }

    private void CloseMenu()
    {
        isMenuOpen = false;
        upgradeMenu.SetActive(false);
    }
}
