using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using TMPro; 

public class UpgradeButtonManager : MonoBehaviour
{
    public Transform buttonContainer;
    public GameObject upgradeButtonPrefab;
    public UpgradeEffect[] possibleUpgrades;

    public float spacingBetweenButtons = -130.0f;

    void Start()
    {
        GenerateUpgradeButtons();
    }

    void GenerateUpgradeButtons()
    {
        // Randomly select three upgrades
        List<UpgradeEffect> selectedUpgrades = new List<UpgradeEffect>();
        int upgradeCount = Mathf.Min(possibleUpgrades.Length, 3);

        while (selectedUpgrades.Count < upgradeCount)
        {
            UpgradeEffect selectedUpgrade = possibleUpgrades[Random.Range(0, possibleUpgrades.Length)];

            if (!selectedUpgrades.Contains(selectedUpgrade))
            {
                selectedUpgrades.Add(selectedUpgrade);
            }
        }

        // Calculate the total height of the buttons to determine the yOffset
        float totalButtonHeight = (upgradeButtonPrefab.GetComponent<RectTransform>().rect.height + spacingBetweenButtons) * (upgradeCount - 1);

        // Initial yOffset should start at the top of the panel
        float yOffset = totalButtonHeight * 0.5f;

        // Create buttons for the selected upgrades
        foreach (UpgradeEffect upgrade in selectedUpgrades)
        {
            GameObject buttonGO = Instantiate(upgradeButtonPrefab, buttonContainer);
            Button button = buttonGO.GetComponent<Button>();
            TextMeshProUGUI buttonText = buttonGO.GetComponentInChildren<TextMeshProUGUI>();

            buttonGO.GetComponent<RectTransform>().anchoredPosition = new Vector2(0, yOffset);

            yOffset -= buttonGO.GetComponent<RectTransform>().rect.height + spacingBetweenButtons; // Decrement yOffset

            buttonText.text = upgrade.upgradeName;

            button.onClick.AddListener(() => ApplyUpgradeEffects(upgrade));
        }
    }


    void ApplyUpgradeEffects(UpgradeEffect upgrade)
    {
        PlayerManagement playerManagement = FindObjectOfType<PlayerManagement>();
        WeaponController weaponController = FindObjectOfType<WeaponController>();
        GunController gunController = FindObjectOfType<GunController>();
        PlayerMove playerMove = FindObjectOfType<PlayerMove>();
        if (upgrade.upgradeName == "Melee Damage +20")
        {
            Debug.Log("Melee Damage +20");
            weaponController.swordDamage += 20;
            PlayerPrefs.SetFloat("SwordDamage", weaponController.swordDamage);
            PlayerPrefs.Save();
            Time.timeScale = 1.0f;
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }
        if (upgrade.upgradeName == "Max Health +20")
        {
            Debug.Log("Max Health +20");
            playerManagement.baseHP += 20;
            // Save the max health
            PlayerPrefs.SetFloat("PlayerMaxHealth", playerManagement.baseHP);
            // Save the current health
            PlayerPrefs.SetFloat("PlayerHealth", playerManagement.currHP);
            PlayerPrefs.Save();
            Time.timeScale = 1.0f;
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }
        if (upgrade.upgradeName == "Mana +20")
        {
            Debug.Log("Mana +20");
            gunController.maxMana += 20;
            PlayerPrefs.SetFloat("PlayerMaxMana", gunController.maxMana);
            PlayerPrefs.Save();
            Time.timeScale = 1.0f;
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }
        if (upgrade.upgradeName == "Movement +4")
        {
            Debug.Log("Movement +4");
            playerMove.moveSpeed += 4;
            PlayerPrefs.SetFloat("PlayerMoveSpeed", playerMove.moveSpeed); 
            PlayerPrefs.Save();
            Time.timeScale = 1.0f;
            SceneManager.LoadScene(SceneManager.GetActiveScene().buildIndex + 1);
        }


    }
}