#include "Bounce.h"
#include "Unit.h"
#include "Emitter.h"
#include "ParticleEngine.h"


Bounce::Bounce()
{
	//Bubble Particles
	m_emBubbles = ParticleEngine::GetInstance()->LoadEmitter("Assets/Particles/BubbleParticles.xml", "Bubbles", m_ptPosition);
}


Bounce::~Bounce()
{
	delete m_emBubbles;
}


void Bounce::CastBounce(Unit* pOther)
{

	if (pOther->GetType() == ENT_PLAYER)
	{
		pOther->SetIsBouncing(true);
	}

}