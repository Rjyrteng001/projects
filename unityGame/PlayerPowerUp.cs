using System.Collections;
using System.Collections.Generic;
using TMPro;
using UnityEngine;

public class PlayerPowerUp : MonoBehaviour
{
    private bool isDamagePowerUpActive = false;
    private bool isSpeedPowerUpActive = false;
    private bool isShieldPowerUpActive = false;
    private bool isHealthPowerUpActive = false;

    private float originalMoveSpeed;
    private float originalMeleeDamage;
    private float originalMagicDamage;
    private float originalHealth;


    private float damageMultiplier;
    private float moveSpeed;

    //reference to RelativeMovement script
    public PlayerMove relativeMovement;
    public WeaponController weaponInfo;
    public PlayerManagement playerHealth;
    public BulletController bulletInfo;

    //reference to the text
    public TMP_Text powerUpPopUpText;

    public void Start()
    {
        originalMoveSpeed = relativeMovement.moveSpeed;
    }
    public void ActivateDamagePowerUp(float duration)
    {
        if (!isDamagePowerUpActive)
        {
            isDamagePowerUpActive = true;
            originalMeleeDamage = weaponInfo.swordDamage;
            originalMagicDamage = bulletInfo.fireballDamage;
            weaponInfo.swordDamage *= 2f; // Double damage
            bulletInfo.fireballDamage *= 2f;

            //display text
            StartCoroutine(DisplayPowerUpPopUp("++ Damage", duration));

            Invoke(nameof(DeactivateDamagePowerUp), duration);
        }
    }

    private void DeactivateDamagePowerUp()
    {
        isDamagePowerUpActive = false;
        weaponInfo.swordDamage = originalMeleeDamage;
        bulletInfo.fireballDamage = originalMagicDamage;

        // Stop the power-up popup coroutine if active
        StopAllCoroutines();
        powerUpPopUpText.text = "";

    }

    public void ActivateSpeedPowerUp(float duration)
    {
        if (!isSpeedPowerUpActive)
        {
            isSpeedPowerUpActive = true;
            originalMoveSpeed = relativeMovement.moveSpeed; // Store original speed
            relativeMovement.moveSpeed *= 2f; // Double speed

            //display text
            StartCoroutine(DisplayPowerUpPopUp("++ Speed", duration));

            Invoke(nameof(DeactivateSpeedPowerUp), duration);
        }
    }

    private void DeactivateSpeedPowerUp()
    {
        isSpeedPowerUpActive = false;
        relativeMovement.moveSpeed = originalMoveSpeed; // Restore original speed

        // Stop the power-up popup coroutine if active
        StopAllCoroutines();
        powerUpPopUpText.text = "";
    }

    public void ActivateShieldPowerUp(float duration)
    {
        if (!isShieldPowerUpActive)
        {
            isShieldPowerUpActive = true;
            playerHealth.EnableShieldEffect(); // Implement shield effect in PlayerManagement
            playerHealth.TakeDamage(0);         // Player takes 0 damage when shield activated

            //display text
            StartCoroutine(DisplayPowerUpPopUp("Shield", duration));

            Invoke(nameof(DeactivateShieldPowerUp), duration);
        }
    }

    private void DeactivateShieldPowerUp()
    {
        isShieldPowerUpActive = false;
        playerHealth.DisableShieldEffect(); // Remove shield effect in PlayerManagement
        playerHealth.AllowNormalDamage(); // Allow normal damage again

        // Stop the power-up popup coroutine if active
        StopAllCoroutines();
        powerUpPopUpText.text = "";
    }

    public void ActivateHealthPowerUp()
    {
        if (!isHealthPowerUpActive)
        {
            isHealthPowerUpActive = true;
            playerHealth.currHP += 50;

            //display text
            StartCoroutine(DisplayPowerUpPopUp("Health", 3f));
        }
    }


    private IEnumerator DisplayPowerUpPopUp(string text, float duration)
    {
        powerUpPopUpText.text = text;
        yield return new WaitForSeconds(duration);
        powerUpPopUpText.text = "";
    }
}


