#include "world.h"

world::world()
{
}

void world::Load(std::string path)
{
	Loader<world> MapLoader;
	MapLoader.LoadData(path, this);
}

void world::Draw()
{
	glColor3d(0.0, 0.0, 0.0);

	for (unsigned int j = 0; j < faces.size(); j++)
	{
		if (MapLoader.TexRange.find(j) != MapLoader.TexRange.end())
		{
			glBindTexture(GL_TEXTURE_2D, MapLoader.TexRange[j]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		if (faces[j].size() == 3)
		{
			glBegin(GL_TRIANGLES);
			for (unsigned int i = 0; i < faces[j].size(); i++)
			{
				glNormal3d(Vn[faces[j][i][2]][0], Vn[faces[j][i][2]][1], Vn[faces[j][i][2]][2]);
				glTexCoord2d(Vt[faces[j][i][1]][0], Vt[faces[j][i][1]][1]);
				glVertex3d(V[faces[j][i][0]][0], V[faces[j][i][0]][1], V[faces[j][i][0]][2]);
			}

			glEnd();
		}
		else
		{
			glBegin(GL_POLYGON);

			for (unsigned int i = 0; i < faces[j].size(); i++)
			{
				glNormal3d(Vn[faces[j][i][2]][0], Vn[faces[j][i][2]][1], Vn[faces[j][i][2]][2]);
				glTexCoord2d(Vt[faces[j][i][1]][0], Vt[faces[j][i][1]][1]);
				glVertex3d(V[faces[j][i][0]][0], V[faces[j][i][0]][1], V[faces[j][i][0]][2]);
			}

			glEnd();
		}
	}

	glutSwapBuffers();
}