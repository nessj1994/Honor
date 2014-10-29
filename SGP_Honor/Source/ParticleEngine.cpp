#include "ParticleEngine.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../TinyXML/tinyxml.h"
#include "../TinyXML/tinystr.h"
#include <cassert>
using namespace std;
//Instantiate the static member for the games instance
ParticleEngine* ParticleEngine::s_pInstance = nullptr;


//////////////////////////////////////////////////////////////////////////
////////////////////////Singleton Accessors//////////////////////////////

//Instantiate ONE instance of the singleton
ParticleEngine* ParticleEngine::GetInstance(void)
{
	if(s_pInstance == nullptr)
	{
		s_pInstance = new ParticleEngine;
	}

	return s_pInstance;
}

//Delete ONE instance of the singleton
void ParticleEngine::DeleteInstance(void)
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////Setup, Play, Cleanup///////////////////////////
bool ParticleEngine::Initialize(void)
{
	
	return true;
}

int ParticleEngine::Update(void)
{

	return 1;
}


void ParticleEngine::Terminate(void)
{
	typedef std::map<std::string, Emitter*>::iterator TheIter;
	for (TheIter iterator = m_mEmitterMap.begin(); iterator != m_mEmitterMap.end(); iterator++)
	{
		SGD::GraphicsManager::GetInstance()->UnloadTexture(iterator->second->GetImage());
		delete iterator->second;
	}


}

/////////////////////////////////////////////////////////////////////////
///////////////////////////////Interface////////////////////////////////
Emitter* ParticleEngine::LoadEmitter(std::string filePath, std::string Name, SGD::Point _Pos)
{
	if (m_mEmitterMap.find(Name.c_str()) != m_mEmitterMap.end())
	{
		return new Emitter(*m_mEmitterMap[Name.c_str()]);
	}
	TiXmlDocument doc(filePath.c_str());
	Emitter* ReturnEmit = new Emitter;
	if (doc.LoadFile())
	{
		TiXmlElement *root = doc.FirstChildElement("Emitter");
		while (root)
		{
			string ImageName = root->Attribute("Particle_Image");
			int EmitterShape;
			root->Attribute("Emitter_Shape", &EmitterShape);
			if (EmitterShape)
			{
				int SpinSpeed;
				root->Attribute("Spin_Speed", &SpinSpeed);
				if (SpinSpeed > 0)
				{
					SpinSpeed += 3;
				}
				int Radius;
				root->Attribute("Radius", &Radius);
				string PinEdges;
				PinEdges = root->Attribute("Pin_Edges");
				if (PinEdges == "True")
				{
					ReturnEmit->PinEdges(true);
				}
				else
				{
					ReturnEmit->PinEdges(false);
				}
				ReturnEmit->SetSpinSpeed(SpinSpeed);
				ReturnEmit->SetRadius(Radius);

			}
			else
			{
				int EWidth;
				root->Attribute("Emitter_Width", &EWidth);
				int EHeight;
				root->Attribute("Emitter_Height", &EHeight);
				ReturnEmit->SetSize({ (float)EWidth, (float)EHeight });
			}
			string isLooping = root->Attribute("Looping");
			bool Looping;
			if (isLooping == "True")
			{
				Looping = true;
			}
			else
			{
				Looping = false;
			}
			int MaxParticles;
			root->Attribute("Max_Particles", &MaxParticles);
			double ParticleScaleX;
			root->Attribute("Particle_Scale_X", &ParticleScaleX);
			double ParticleScaleY;
			root->Attribute("Particle_Scale_Y", &ParticleScaleY);
			int StartColorA;
			root->Attribute("Start_Color_A", &StartColorA);
			int StartColorR;
			root->Attribute("Start_Color_R", &StartColorR);
			int StartColorG;
			root->Attribute("Start_Color_G", &StartColorG);
			int StartColorB;
			root->Attribute("Start_Color_B", &StartColorB);
			int EndColorA;
			root->Attribute("End_Color_A", &EndColorA);
			int EndColorR;
			root->Attribute("End_Color_R", &EndColorR);
			int EndColorG;
			root->Attribute("End_Color_G", &EndColorG);
			int EndColorB;
			root->Attribute("End_Color_B", &EndColorB);
			int ColorChangeScale;
			root->Attribute("Color_Change_Scale", &ColorChangeScale);
			int AlphaFade;
			root->Attribute("Alpha_Fade", &AlphaFade);
			double Rotation;
			root->Attribute("Rotation", &Rotation);
			int Gravity;
			root->Attribute("Gravity", &Gravity);
			//Change This Later
			ReturnEmit->SetPosition(_Pos);
			//
			if (ImageName.size() > 0)
			{
				string path = "Assets/Particles/";
				path += ImageName.c_str();
				ReturnEmit->SetImage(SGD::GraphicsManager::GetInstance()->LoadTexture(path.c_str()));
			}
			else
			{
				ReturnEmit->SetImage(SGD::INVALID_HANDLE);
			}
			ReturnEmit->SetEmitterShape(EmitterShape);
			ReturnEmit->SetLooping(Looping);
			ReturnEmit->SetMaxParticles(MaxParticles);
			ReturnEmit->SetAlphaStart((float)StartColorA);
			ReturnEmit->SetAlphaFade((float)AlphaFade);
			ReturnEmit->SetGravity((float)Gravity);
			ReturnEmit->SetScale({ (float)ParticleScaleX, (float)ParticleScaleY });
			ReturnEmit->SetRotation((float)Rotation);
			ReturnEmit->SetColorChange((float)ColorChangeScale);
			ReturnEmit->SetStartColor({ (unsigned char)StartColorA, (unsigned char)StartColorR, (unsigned char)StartColorG, (unsigned char)StartColorB });
			ReturnEmit->SetEndColor({ (unsigned char)EndColorA, (unsigned char)EndColorR, (unsigned char)EndColorG, (unsigned char)EndColorB });


			TiXmlElement *MaxParticle = root->FirstChildElement("Max_Particles");
			if (MaxParticle)
			{
				int VelocityX;
				MaxParticle->Attribute("Velocity_X", &VelocityX);
				int VelocityY;
				MaxParticle->Attribute("Velocity_Y", &VelocityY);
				double MaxLifeSpan;
				MaxParticle->Attribute("Life_Span", &MaxLifeSpan);
				ReturnEmit->SetMaxVelocity({ (float)VelocityX, (float)VelocityY });
				ReturnEmit->SetMaxLifeSpan((float)MaxLifeSpan);

			}
			TiXmlElement *MinParticle = MaxParticle->NextSiblingElement("Min_Particles");
			if (MinParticle)
			{
				int VelocityX;
				MinParticle->Attribute("Velocity_X", &VelocityX);
				int VelocityY;
				MinParticle->Attribute("Velocity_Y", &VelocityY);
				double MinLifeSpan;
				MinParticle->Attribute("Life_Span", &MinLifeSpan);
				ReturnEmit->SetMinVelocity({ (float)VelocityX, (float)VelocityY });
				ReturnEmit->SetMinLifeSpan((float)MinLifeSpan);
				ReturnEmit->StartParticles();
				m_mEmitterMap[Name] = ReturnEmit;
				return new Emitter(*ReturnEmit);
			}
		}

		return NULL;
	}
	else
	{
		assert(doc.LoadFile() != 0 && "Particle Engine XML File was not loaded!(Possible Incorrect Path)<-------------------------------------");
		return NULL;
	}
	
}

