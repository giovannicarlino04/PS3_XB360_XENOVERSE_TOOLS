#include "EAN.h"
#include "ESKOgre.h"

#ifndef EANOGRE_H_INCLUDED
#define EANOGRE_H_INCLUDED


class EANOgre;

class EANOgreAnimation
{
protected:
	string name;
	EANOgre* parent;
	EANAnimation* eanAnim;
	EmaAnimation* emaAnim;
	size_t index;

public:
	EANOgreAnimation(string name, EANOgre* parent, size_t index = -1, EANAnimation* eanAnim = 0, EmaAnimation* emaAnim = 0) { this->name = name; this->parent = parent; this->index = index; this->eanAnim = eanAnim; this->emaAnim = emaAnim; };

	size_t getIndex() { return index; }
	string getName() { return name; }
	EANOgre* getParent() { return parent; }

	EANAnimation* getEanAnimation() { return eanAnim; };
	EmaAnimation* getEmaAnimation() { return emaAnim; };
};


class EANOgre
{
protected:
	EAN* ean;
	EMA* ema;

	string name;
	std::vector<EANOgreAnimation> mListEANOgreAnimation;

	bool animation_resources_created;
	ESKOgre* esk_skeleton;
	float fps;
	EANOgreAnimation* force_animation;
	EANOgreAnimation* force_animation2;			//Todo do a list and change action to "Add animation to play" instead of "Play second animation"

public:
	EANOgre();
	~EANOgre(void);

	bool load(string filename);
	void clear();

	string getName() { return name; }
	std::vector<EANOgreAnimation> &getAnimations() { return mListEANOgreAnimation; }

	void createOgreAnimations(ESKOgre* v);
	Ogre::Animation *createOgreAnimation(EANAnimation* animation);
	void	createOgreAnimationTracks(Ogre::Animation* mAnim, Ogre::Skeleton *ogre_skeleton, EANAnimation* animation);
	
	Ogre::Vector3 giveAngleOrientationForThisOrientationTaitBryan(Ogre::Quaternion orient);

	void tagForceAnimation(EANOgreAnimation *v) { force_animation = v; }
	void tagForceAnimation2(EANOgreAnimation *v) { force_animation2 = v; }
	EANOgreAnimation* toForceAnimation();
	EANOgreAnimation* toForceAnimation2();
};

#endif