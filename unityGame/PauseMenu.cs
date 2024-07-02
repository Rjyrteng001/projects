using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Rendering;
using UnityEngine.SceneManagement;
using UnityEngine.UI;

public class PauseMenu : MonoBehaviour
{
    public Slider volumeSlider;
    public AudioSource gameAudio;
    public Toggle muteToggle;

    private float previousVolume;

    public GameObject pauseMenuUI;
    public GameObject OptionsMenuUI;

    private bool isPaused = false;

    private void Start()
    {
        volumeSlider.value = gameAudio.volume;
        volumeSlider.onValueChanged.AddListener(UpdateVolume);

        muteToggle.onValueChanged.AddListener(OnToggleMute);

        previousVolume = volumeSlider.value;
    }

    private void Update()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            TogglePause();
        }
    }

    public void TogglePause()
    {
        isPaused = !isPaused;

        if (isPaused)
        {
            Time.timeScale = 0f; // Pause the game
            pauseMenuUI.SetActive(true);
        }
        else
        {
            Time.timeScale = 1f; // Resume the game
            pauseMenuUI.SetActive(false);
        }
    }

    public void OptionsMenu()
    {
        pauseMenuUI.SetActive(false);
        OptionsMenuUI.SetActive(true);
    }

    public void BackToStartScreen()
    {
        TogglePause();
        SceneManager.LoadScene(0);
    }

    public void BackButton()
    {
        OptionsMenuUI.SetActive(false);
        pauseMenuUI.SetActive(true);
    }

    public void UpdateVolume(float newVolume)
    {
        if (!muteToggle.isOn)
        {
            gameAudio.volume = newVolume;
        }

        // Update the previousVolume regardless of mute state
        previousVolume = newVolume;
    }

    private void OnToggleMute(bool isMuted)
    {
        if (isMuted)
        {
            previousVolume = gameAudio.volume;
            gameAudio.volume = 0.0f;
        }

        else
        {
            gameAudio.volume = previousVolume;
        }
    }
}