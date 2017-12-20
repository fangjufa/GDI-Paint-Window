using UnityEngine;
using System.Collections;
using System.Runtime.InteropServices;


public class CSharpNewBehaviour: MonoBehaviour {

    public RenderTexture texture;

    private const string dllName = "WindowTest";

    private bool shutdown;
    private Texture2D texture2D;

    private byte[] textureDatas;
    // Use this for initialization
    void Start ()
    {
        texture2D = new Texture2D(texture.width, texture.height,TextureFormat.BGRA32,true);       
    }

    void Update()
    {
        RenderTexture.active = texture;
        texture2D.ReadPixels(new Rect(0, 0, texture.width, texture.height),0,0,true);
        texture2D.Apply();
        SetTexture(texture2D.GetPixels32());
        RenderTexture.active = null;
    }

    void OnGUI()
    {
        if(GUILayout.Button("打开窗口"))
        {
            if (OpenDXWind("aa.bmp",texture2D.GetPixels32(),texture.width,texture.height))
            {
                shutdown = false;
                Debug.Log("Success");
                //StartCoroutine(Rendering());
            }
            else
                Debug.Log("Failed");
        }
        if (GUILayout.Button("关闭窗口"))
        {
            shutdown = true;
            CloseDXWind();
        }
    }

    //DLL的名字不能带.dll，不然会找不到该dll
    [DllImport(dllName)]
    public extern static bool OpenDXWind(string fileName, Color32[] texturePtr, int width, int height);
    //public extern static bool OpenDXWind(string fileName,System.IntPtr texturePtr,int width,int height);

    [DllImport(dllName)]
    public extern static bool CloseDXWind();

    [DllImport(dllName)]
    public extern static bool SetTexture(Color32[] texturePtr);
}