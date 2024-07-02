using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class DamagePopupController : MonoBehaviour
{
    public TextMeshProUGUI damageText;
    public float popupDuration = 1.0f;
    public float popupSpeed = 1.0f;

    public Vector3 popupOffset = new Vector3(0f, 1.5f, 0f);

    private void Start()
    {
        // Start the animation
        StartCoroutine(PopupAnimation());
    }

    public void ShowDamagePopup(float damageValue)
    {
        // Set the damage value in the popup text
        damageText.text = damageValue.ToString();
    }

    private IEnumerator PopupAnimation()
    {
        float elapsedTime = 0;

        while (elapsedTime < popupDuration)
        {
            // Move the popup upwards
            transform.Translate(Vector3.up * popupSpeed * Time.deltaTime);

            // Fade out the popup text
            Color textColor = damageText.color;
            textColor.a = Mathf.Lerp(1f, 0f, elapsedTime / popupDuration);
            damageText.color = textColor;

            elapsedTime += Time.deltaTime;
            yield return null;
        }

        // Destroy the popup GameObject when the animation is finished
        Destroy(gameObject);
    }

}
