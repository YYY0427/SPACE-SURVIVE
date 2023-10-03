using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEditor;
using System.IO;

public class CustomMenu 
{
    enum ObjectType
    {
        Mesh, 
    }
    public static void WriteVector3(BinaryWriter bw, Vector3 vec)
    {
        bw.Write(vec.x);
        bw.Write(vec.y);
        bw.Write(vec.z);
    }

    [MenuItem("CustomMenu/ObjectData Exportar")]
    public static void Func()
    {
        var rootObject = Selection.activeGameObject;
        if(rootObject == null)
        {
            EditorUtility.DisplayDialog("�z�u�f�[�^���o�͂ł��܂���", "�z�u�f�[�^���o�͂���ɂ́A���[�g�I�u�W�F�N�g��I�����Ă�������", "����");
            return;
        }
        else
        {
            // SaveFilePanel�̓Z�[�u���Ă���Ȃ��B�������Ԃ�����
            // �Z�[�u�_�C�A���O���o���āA���[�U���I�񂾏o�̓t�H���_���Ԃ����
            var path = EditorUtility.SaveFilePanel("�z�u�f�[�^�o��", ".", "ObjectData.dat", "dat");
            var stream = File.Create(path);
            var bw = new BinaryWriter(stream);
           
            bw.Write(rootObject.transform.childCount);

            // �����̎q�I�u�W�F�N�g���������邩�𒲂ׂĎq���̍��W���o�͂���
            for(int i = 0; i < rootObject.transform.childCount; i++)
            {
                var gameObject = rootObject.transform.GetChild(i);

                // �I�u�W�F�N�g��
                bw.Write(gameObject.name);

                // �ʒu���̏o��
                var pos = gameObject.transform.position;
                WriteVector3(bw, pos);

                // ��]�����o��
                // �x���@�ŏo��
                var eular = gameObject.transform.eulerAngles;
                WriteVector3(bw, eular);

                // �g����̏o��
                var scale = gameObject.transform.localScale;
                WriteVector3(bw, scale);
            }

            bw.Close();
            stream.Close(); ;
            Debug.Log("ObjectData Exportar�̎��s��������܂���");
            EditorUtility.DisplayDialog("�z�u�f�[�^���o�͂���܂���", path + "�ɏo�͂���܂���", "����");
        }
    }
}
