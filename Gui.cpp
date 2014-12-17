Gui::Gui ()
{
    addAndMakeVisible (toets = new ImageButton ("LFPN"));
    toets->addListener (this);

    LFPN->setImages (false, true, true,
                     ImageCache::getFromMemory (switcherUp_png, switcherUp_pngSize), 1.000f, Colour (0x00000000),
                     ImageCache::getFromMemory (switcherUp_png, switcherUp_pngSize), 1.000f, Colour (0x00000000),
                     ImageCache::getFromMemory (switcherDown_png, switcherDown_pngSize), 1.000f, Colour (0x00000000));
    addAndMakeVisible (LFEnable = new ImageButton ("LFEnable"));
    LFEnable->addListener (this);
