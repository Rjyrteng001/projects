using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class EndGameButton : MonoBehaviour
{
    public void loadScene()
    {
        Time.timeScale = 1.0f;
        SceneManager.LoadScene(0);
    }
}
