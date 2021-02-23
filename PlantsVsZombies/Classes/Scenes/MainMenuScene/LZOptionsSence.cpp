/**
 *Copyright (c) 2019 LZ.All Right Reserved
 *Author : LZ
 *Date: 2019.7.17
 *Modify: 2020.2.3
 *Email: 2117610943@qq.com
 */

#include "LZOptionsSence.h"
#include "Based/LZGlobalVariable.h"
#include "Based/LZUserData.h"
#include "Based/LZPlayMusic.h"

OptionsMenu::OptionsMenu():
	_userDefault(UserDefault::getInstance()),
	_director(Director::getInstance())
{
}
 
OptionsMenu::~OptionsMenu()
{
}

bool OptionsMenu::init()
{
	if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))return false;

	createShieldLayer(this);

	this->createDialog();

	return true;
}

void OptionsMenu::createDialog()
{
	_option = Sprite::createWithSpriteFrameName("options_menuback.png");
	_option->setPosition(_director->getWinSize() / 2);
	_option->setScale(1.7f);
	_option->setName("_option");
	this->addChild(_option);
	
	/* �����ƶ����� */
	createTouchtListener(_option);

	/* ���������� */
	this->createSlider(Vec2(250, 350), Vec2(140, 350), _global->userInformation->getGameText().find("����")->second->text, OptionScene_Slider::����,
		Sprite::createWithSpriteFrameName("options_sliderslot.png"),
		Sprite::createWithSpriteFrameName("options_sliderslot.png"),
		Sprite::createWithSpriteFrameName("options_sliderknob2.png"),
		Sprite::createWithSpriteFrameName("options_sliderknob2.png"));
	this->createSlider(Vec2(250, 310), Vec2(140, 310), _global->userInformation->getGameText().find("��Ч")->second->text, OptionScene_Slider::��Ч, 
		Sprite::createWithSpriteFrameName("options_sliderslot.png"),
		Sprite::createWithSpriteFrameName("options_sliderslot.png"),
		Sprite::createWithSpriteFrameName("options_sliderknob2.png"),
		Sprite::createWithSpriteFrameName("options_sliderknob2.png"));

	/* ������ѡ�� */
	this->createCheckBox(Vec2(300, 280), Vec2(140, 280), _global->userInformation->getGameText().find("��Ϣ")->second->text, OptionScene_CheckBox::��ʾ��Ϣ, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 245), Vec2(140, 245), _global->userInformation->getGameText().find("ȫ��")->second->text, OptionScene_CheckBox::ȫ��, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 210), Vec2(140, 210), _global->userInformation->getGameText().find("��֡��")->second->text, OptionScene_CheckBox::��֡��, "options_checkbox0", "options_checkbox1");
	this->createCheckBox(Vec2(300, 175), Vec2(140, 175), _global->userInformation->getGameText().find("������ʾ")->second->text, OptionScene_CheckBox::������ʾ, "options_checkbox0", "options_checkbox1");

	/* �����ʵ� */
	if (_global->userInformation->getIsShowEggs() &&
		(UserData::getInstance()->openIntUserData(const_cast<char*>(StringUtils::format(
			_global->userInformation->getSystemCaveFileName().c_str(), 1).c_str())) >= 52 ||
		UserData::getInstance()->openIntUserData(const_cast<char*>(StringUtils::format(
			_global->userInformation->getSystemDifCaveFileName().c_str(), 1).c_str())) >= 52))
	{
		auto button = Button::create("button.png", "button_down.png", "", TextureResType::PLIST);
		button->setTitleLabel(label(
			_global->userInformation->getGameText().find("������")->second->text,
			_global->userInformation->getGameText().find("������")->second->fontsize, Vec2(0, 0), 0, Color3B::GRAY, 0.5f));
		button->setPosition(Vec2(210, 140));
		button->setScaleX(2.0f);
		_option->addChild(button);
		button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
			{
				switch (type)
				{
				case Widget::TouchEventType::BEGAN:
					PlayMusic::playMusic("gravebutton");
					break;
				case Widget::TouchEventType::ENDED:
					Director::getInstance()->pushScene(TransitionFade::create(0.5f, GameEasterEggs::create()));
					break;
				}
			});
	}

    /* ������ť */
	this->createButton();
}

ControlSlider* OptionsMenu::createSlider(Vec2& vec2, Vec2& vec2_, const std::string& Label, OptionScene_Slider slider_type,
	Sprite* BgFile, Sprite* progressFile, Sprite* thumbFile, Sprite* selectthumbFile, const bool IsNew)
{
	/* ���������� */
	ControlSlider* slider; ;
	selectthumbFile ? selectthumbFile->setColor(Color3B::GRAY),
		slider = ControlSlider::create(BgFile, progressFile, thumbFile, selectthumbFile) :
		slider = ControlSlider::create(BgFile, progressFile, thumbFile);

	slider->setMinimumValue(0);
	slider->setMaximumValue(100);
	slider->setPosition(vec2);
	_option->addChild(slider);

	switch (slider_type)
	{
	case OptionScene_Slider::����:
		slider->setValue(_global->userInformation->getBackGroundMusicVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::backGroundMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	case OptionScene_Slider::��Ч:
		slider->setValue(_global->userInformation->getSoundEffectVolume() * 100);
		slider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsMenu::soundEffectMusicVolumeChangeCallBack), Control::EventType::VALUE_CHANGED);
		break;
	}

	/* ������ǩ */
	switch (IsNew)
	{
	case true:
		_option->addChild(label(Label, 30, vec2_, 0, Color3B::WHITE));
		break;
	case false:
		_option->addChild(label(Label, 20, vec2_, 0, Color3B::GRAY));
		break;
	}

	return slider;
}

void OptionsMenu::backGroundMusicVolumeChangeCallBack(Ref* sender, Control::EventType type)
{
	auto slider = (ControlSlider*)sender;
	for (auto sp : _global->userInformation->getBackgroundMusic())  /* ѭ�������������� */
	{
		AudioEngine::setVolume(sp, slider->getValue() / 100.0f);
	}

	_global->userInformation->setBackGroundMusicVolume(slider->getValue() / 100.0f);
	_userDefault->setFloatForKey("GLOBALMUSIC", _global->userInformation->getBackGroundMusicVolume());

	if (slider->getSelectedThumbSprite()->getContentSize().height > 30)
	{
		/* ��ת���� */
		slider->getThumbSprite()->setRotation(slider->getValue() * 10);
		slider->getSelectedThumbSprite()->setRotation(slider->getValue() * 10);
	}
}

void OptionsMenu::soundEffectMusicVolumeChangeCallBack(Ref* sender, Control::EventType type)
{
	/* ������Ч���� */
	auto slider = (ControlSlider*)sender;
	_global->userInformation->setSoundEffectVolume(slider->getValue() / 100.0f);
	_userDefault->setFloatForKey("SOUNDEFFECT", _global->userInformation->getSoundEffectVolume());

	if (slider->getSelectedThumbSprite()->getContentSize().height > 30)
	{
		/* ��ת���� */
		slider->getThumbSprite()->setRotation(slider->getValue() * 10);
		slider->getSelectedThumbSprite()->setRotation(slider->getValue() * 10);
	}
}

void OptionsMenu::deleteDialog()
{
	setMouseListenerEnable();
	this->removeFromParent();
}

CheckBox* OptionsMenu::createCheckBox(Vec2 &vec2, Vec2 &vec2_, const std::string &Label, OptionScene_CheckBox button_type,
	const std::string NotSelect, const std::string Select, const bool IsNew)
{
	auto checkbox = CheckBox::create();
	checkbox->loadTextureBackGround(NotSelect + ".png", TextureResType::PLIST);
	checkbox->loadTextureFrontCross(Select + ".png", TextureResType::PLIST);
	checkbox->setPosition(vec2);
	_option->addChild(checkbox);

	switch (button_type)
	{
	case OptionScene_CheckBox::��ʾ��Ϣ:
		checkbox->setSelected(_global->userInformation->getIsShowInformation() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::ȫ��:
		checkbox->setSelected(_global->userInformation->getIsSelectFullScreen() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::��֡��:
		checkbox->setSelected(_global->userInformation->getIsSelectHighFPS() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::�������:
		checkbox->setSelected(_global->userInformation->getIsSelectCursorNotHide() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::������ʾ:
		checkbox->setSelected(_global->userInformation->getIsSelectStretchingShow() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::���붯��:
		checkbox->setSelected(_global->userInformation->getIsEaseAnimation() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	case OptionScene_CheckBox::��ֱͬ��:
		checkbox->setSelected(_global->userInformation->getIsVerticalSynchronization() == cocos2d::ui::CheckBox::EventType::SELECTED ? true : false);
		break;
	default:
		break;
	}
	
	checkbox->addEventListener([=](Ref* sender, CheckBox::EventType type)
	{
		switch (type)
		{
		case CheckBox::EventType::SELECTED:
			switch (button_type)
			{
			case OptionScene_CheckBox::��ʾ��Ϣ: /* ��ʾ��Ϣ */
				_userDefault->setBoolForKey("SHOWINFORMATION", true);
				_global->userInformation->setIsShowInformation(CheckBox::EventType::SELECTED); /* ���� */
				_director->setDisplayStats(true);
				break;
			case OptionScene_CheckBox::ȫ��: /* ȫ�� */
				_userDefault->setBoolForKey("SHOWFULLSCREEN", true);
				_global->userInformation->setIsSelectFullScreen(CheckBox::EventType::SELECTED);
				((GLViewImpl*)_director->getOpenGLView())->setFullscreen();
				break;
			case OptionScene_CheckBox::��֡��: /* ��֡�� */
				_userDefault->setBoolForKey("SHOWHIGHFPS", true);
				_global->userInformation->setIsSelectHighFPS(CheckBox::EventType::SELECTED);
				_director->setAnimationInterval(1.0f / UserInformation::getScreenDisplayFrequency());
				break;
			case OptionScene_CheckBox::�������: /* ������� */
				UserData::getInstance()->caveUserData("CURSORHIDE", true);
				_global->userInformation->setIsSelectCursorNotHide(CheckBox::EventType::SELECTED);
				break;
			case OptionScene_CheckBox::������ʾ: /* ������ʾ */
				_userDefault->setBoolForKey("STRETCHINGSHOW", true);
				_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::EXACT_FIT);
				_global->userInformation->setIsSelectStretchingShow(CheckBox::EventType::SELECTED);
				break;
			case OptionScene_CheckBox::���붯��: 
				UserData::getInstance()->caveUserData("EASEANIMATION", true);
				_global->userInformation->setIsEaseAnimation(CheckBox::EventType::SELECTED);
				break;
			case OptionScene_CheckBox::��ֱͬ��:
				_userDefault->setBoolForKey("VERTICALSYNCHRONIZATION", true);
				wglSwapIntervalEXT(1);
				_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::SELECTED);
				break;
			default:
				break;
			}
			break; 
		case CheckBox::EventType::UNSELECTED:
			switch (button_type)
			{
			case OptionScene_CheckBox::��ʾ��Ϣ:
				_userDefault->setBoolForKey("SHOWINFORMATION", false);
				_global->userInformation->setIsShowInformation(CheckBox::EventType::UNSELECTED); /* ���� */
				_director->setDisplayStats(false);
				break;
			case OptionScene_CheckBox::ȫ��:
				_userDefault->setBoolForKey("SHOWFULLSCREEN", false);
				_global->userInformation->setIsSelectFullScreen(CheckBox::EventType::UNSELECTED);
				((GLViewImpl*)_director->getOpenGLView())->setWindowed(1280, 720);
				break;
			case OptionScene_CheckBox::��֡��:
				_userDefault->setBoolForKey("SHOWHIGHFPS", false);
				_global->userInformation->setIsSelectHighFPS(CheckBox::EventType::UNSELECTED);
				_director->setAnimationInterval(1.0f / 30);
				break;
			case OptionScene_CheckBox::�������:
				UserData::getInstance()->caveUserData("CURSORHIDE", false);
				_global->userInformation->setIsSelectCursorNotHide(CheckBox::EventType::UNSELECTED);
				break;
			case OptionScene_CheckBox::������ʾ:
				_userDefault->setBoolForKey("STRETCHINGSHOW", false);
				_director->getOpenGLView()->setDesignResolutionSize(_director->getWinSize().width, _director->getWinSize().height, ResolutionPolicy::SHOW_ALL);
				_global->userInformation->setIsSelectStretchingShow(CheckBox::EventType::UNSELECTED);
				break;
			case OptionScene_CheckBox::���붯��:
				UserData::getInstance()->caveUserData("EASEANIMATION", false);
				_global->userInformation->setIsEaseAnimation(CheckBox::EventType::UNSELECTED);
				break;
			case OptionScene_CheckBox::��ֱͬ��:
				_userDefault->setBoolForKey("VERTICALSYNCHRONIZATION", false);
				wglSwapIntervalEXT(0);
				_global->userInformation->setIsVerticalSynchronization(CheckBox::EventType::UNSELECTED);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	});

	switch (IsNew)
	{
	case true:
		_option->addChild(label(Label, 30, vec2_, 0, Color3B::WHITE));
		break;
	case false:
		_option->addChild(label(Label, 20, vec2_, 0, Color3B::GRAY));
		break;
	}
	
	return checkbox;
}

void OptionsMenu::createButton()
{
	auto button = Button::create("options_backtogamebutton0.png","options_backtogamebutton2.png","",TextureResType::PLIST);
	button->setTitleLabel(label(
		_global->userInformation->getGameText().find("����")->second->text, 
		_global->userInformation->getGameText().find("����")->second->fontsize));
	button->setPosition(Vec2(210, 55));
	_option->addChild(button);
	button->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::BEGAN:
			PlayMusic::playMusic("gravebutton");
			break;
		case Widget::TouchEventType::ENDED:
			deleteDialog();
			break;
		}
	});
}