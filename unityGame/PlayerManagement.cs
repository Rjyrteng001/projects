using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;

public class PlayerManagement : MonoBehaviour
{
    public float baseHP;
    public float currHP;

    public Slider healthBarSlider;

    private bool isShieldActive = false;
    private bool allowNormalDamage = true;
    public PlayerMove move;

    public Canvas gameOverCanvas;

    // Start is called before the first frame update
    void Start()
    {
        if (gameOverCanvas != null)
        {
            gameOverCanvas.gameObject.SetActive(false);
        }

        currHP = PlayerPrefs.GetFloat("PlayerHealth", currHP);
        baseHP = PlayerPrefs.GetFloat("PlayerMaxHealth", baseHP);
        updateHealthBar();
    }
    private void Update()
    {
        updateHealthBar();
    }
    private void OnDestroy()
    {
        // Save current health when the object is destroyed
        PlayerPrefs.SetFloat("PlayerHealth", currHP);
        PlayerPrefs.SetFloat("PlayerMaxHealth", baseHP);
        PlayerPrefs.Save();
    }

    public void TakeDamage(float damage)
    {
        if (!isShieldActive && allowNormalDamage && !move.isDashing)
        {
            currHP -= damage;
            Debug.Log("Hit, player at " + currHP + "HP");
            updateHealthBar();
        }
        else
        {
            Debug.Log("Shield absorbed damage");
        }

        if (currHP <= 0)
        {
            if (gameOverCanvas != null)
            {
                gameOverCanvas.gameObject.SetActive(true);
            }

            Time.timeScale = 0.0f;
        }
    }

    public void AllowNormalDamage()
    {
        allowNormalDamage = true;
    }

    public void EnableShieldEffect()
    {
        isShieldActive = true;
    }

    public void DisableShieldEffect()
    {
        isShieldActive = false;
    }

    public void updateHealthBar()
    {
        if (healthBarSlider != null)
        {
            // Calculate the ratio of current health to max health
            float healthRatio = currHP / baseHP;

            // Update the health bar slider's max value and current value
            healthBarSlider.maxValue = baseHP;
            healthBarSlider.value = currHP;
        }
    }
}

