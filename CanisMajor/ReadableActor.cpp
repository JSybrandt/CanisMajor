#include "ReadableActor.h"
#include "CanisMajor.h"

void ReadableActor::interactWith(Camera* player)
{
	if(concealedItem != nullptr)
	{
		Vector3 pos= concealedItem->getPosition();
		Vector3 rot= concealedItem->getRotation();
		concealedItem->create(pos,rot);
		concealedItem = nullptr;
		game->setNoteText(duration, myText);
	}
	else
	{
		game->setNoteText(duration, myText);
	}
}
void ReadableActor::create(Vector3 pos, Vector3 rot, Vector3 scale, Actor* item)
{
	Actor::create(pos);
	setRotation(rot);
	setScale(scale);
	concealedItem = item;
	if(item!=nullptr)
		item->isVisible = false;
	targeted = false;
}

void ReadableActor::update(float dt) {
	Actor::update(dt);
	targeted = false;

}

void ReadableActor::draw(ID3D10EffectMatrixVariable* fx, Matrix& camera, Matrix& projection, ID3D10EffectTechnique* mTech) {
	game->mfxHighlightBool->SetBool(targeted);
	Actor::draw(fx, camera, projection, mTech);
}