/*
  ==============================================================================

   This file is part of the JUCE library - "Jules' Utility Class Extensions"
   Copyright 2004-10 by Raw Material Software Ltd.

  ------------------------------------------------------------------------------

   JUCE can be redistributed and/or modified under the terms of the GNU General
   Public License (Version 2), as published by the Free Software Foundation.
   A copy of the license is included in the JUCE distribution, or can be found
   online at www.gnu.org/licenses.

   JUCE is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
   A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

  ------------------------------------------------------------------------------

   To release a closed-source product which uses JUCE, commercial licenses are
   available: visit www.rawmaterialsoftware.com/juce for more information.

  ==============================================================================
*/

#include "../jucer_Headers.h"
#include "jucer_StoredSettings.h"


//==============================================================================
StoredSettings::StoredSettings()
{
    flush();
}

StoredSettings::~StoredSettings()
{
    flush();
    props = nullptr;
    clearSingletonInstance();
}

juce_ImplementSingleton (StoredSettings);


//==============================================================================
PropertiesFile& StoredSettings::getProps()
{
    jassert (props != nullptr);
    return *props;
}

void StoredSettings::flush()
{
    if (props != nullptr)
    {
        props->setValue ("recentFiles", recentFiles.toString());

        props->removeValue ("keyMappings");

        if (commandManager != nullptr)
        {
            ScopedPointer <XmlElement> keys (commandManager->getKeyMappings()->createXml (true));

            if (keys != nullptr)
                props->setValue ("keyMappings", (XmlElement*) keys);
        }
    }

    props = nullptr;

    {
        // These settings are used in defining the properties file's location.
        PropertiesFile::Options options;
        options.applicationName     = "Introjucer";
        options.folderName          = "Introjucer";
        options.filenameSuffix      = "settings";
        options.osxLibrarySubFolder = "Application Support";

        props = new PropertiesFile (options);

        // Because older versions of the introjucer saved their settings under a different
        // name, this code is an example of how to migrate your old settings files...
        if (! props->getFile().exists())
        {
            PropertiesFile::Options oldOptions;
            oldOptions.applicationName      = "Jucer2";
            oldOptions.filenameSuffix       = "settings";
            oldOptions.osxLibrarySubFolder  = "Preferences";

            PropertiesFile oldProps (oldOptions);

            if (oldProps.getFile().exists())
                props->addAllPropertiesFrom (oldProps);
        }
    }

    // recent files...
    recentFiles.restoreFromString (props->getValue ("recentFiles"));
    recentFiles.removeNonExistentFiles();

    // swatch colours...
    swatchColours.clear();

    #define COL(col)  Colours::col,

    const Colour colours[] =
    {
        #include "jucer_Colours.h"
        Colours::transparentBlack
    };

    #undef COL

    const int numSwatchColours = 24;

    for (int i = 0; i < numSwatchColours; ++i)
    {
        Colour defaultCol (colours [2 + i]);
        swatchColours.add (Colour (props->getValue ("swatchColour" + String (i),
                                                    hexString8Digits (defaultCol.getARGB())).getHexValue32()));
    }
}

const Array<File> StoredSettings::getLastProjects() const
{
    StringArray s;
    s.addTokens (props->getValue ("lastProjects"), "|", "");

    Array<File> f;
    for (int i = 0; i < s.size(); ++i)
        f.add (File (s[i]));

    return f;
}

void StoredSettings::setLastProjects (const Array<File>& files)
{
    StringArray s;
    for (int i = 0; i < files.size(); ++i)
        s.add (files.getReference(i).getFullPathName());

    props->setValue ("lastProjects", s.joinIntoString ("|"));
}

const File StoredSettings::getLastKnownJuceFolder() const
{
    File defaultJuceFolder (FileHelpers::findDefaultJuceFolder());
    File f (props->getValue ("lastJuceFolder", defaultJuceFolder.getFullPathName()));

    if ((! FileHelpers::isJuceFolder (f)) && FileHelpers::isJuceFolder (defaultJuceFolder))
        f = defaultJuceFolder;

    return f;
}

void StoredSettings::setLastKnownJuceFolder (const File& file)
{
    jassert (FileHelpers::isJuceFolder (file));
    props->setValue ("lastJuceFolder", file.getFullPathName());
}

const StringArray& StoredSettings::getFontNames()
{
    if (fontNames.size() == 0)
        fontNames = Font::findAllTypefaceNames();

    return fontNames;
}

const Image StoredSettings::getFallbackImage()
{
    if (fallbackImage.isNull())
        fallbackImage = ImageFileFormat::loadFrom (BinaryData::juce_icon_png, BinaryData::juce_icon_pngSize);

    return fallbackImage;
}

const Drawable* StoredSettings::getImageFileIcon()
{
    if (imageFileIcon == nullptr)
    {
        static const unsigned char data[] =
        { 120,218,197,90,75,111,28,199,17,158,181,60,146,98,36,135,28,98,58,185,100,175,1,164,65,87,117,87,63,142,74,156,228,34,3,129,125,80,146,27,37,46,229,141,41,238,130,92,37,240,191,207,87,61,59,195,158,215,138,176,44,154,132,128,209,246,199,154,238,122,126,
        85,189,95,127,251,226,213,139,63,191,252,107,181,250,236,250,252,221,166,90,125,81,127,125,115,254,191,243,215,87,155,106,123,81,173,126,85,111,94,191,124,241,54,189,170,86,171,191,223,236,222,239,171,213,231,237,231,251,237,155,195,251,155,77,117,216,
        237,95,110,46,15,213,234,113,109,158,173,141,254,255,219,237,219,239,241,193,211,154,156,215,143,94,239,14,135,221,187,22,245,84,81,36,161,90,61,234,94,116,91,85,171,223,252,227,252,240,125,181,170,85,246,231,245,158,170,219,195,205,238,135,205,171,237,
        133,126,252,168,54,213,127,118,219,235,195,119,135,31,177,177,213,147,250,221,246,176,185,169,222,156,239,143,159,60,174,95,191,63,28,170,235,221,245,191,55,55,187,87,219,235,139,237,245,219,246,15,87,143,254,182,189,186,194,1,15,63,238,243,223,222,238,
        174,240,154,55,187,171,221,251,27,61,240,229,241,167,170,190,203,47,61,9,53,249,167,170,242,126,171,213,111,191,217,253,23,200,21,118,188,250,170,246,13,73,178,209,134,240,108,237,26,203,137,99,114,174,170,94,110,175,23,64,212,24,31,133,3,159,0,133,208,
        68,19,130,79,241,4,136,200,53,226,124,74,97,0,58,27,130,132,26,142,158,221,8,99,83,227,99,48,108,121,25,19,108,195,176,167,55,126,25,67,198,55,193,177,53,39,228,144,51,13,99,159,206,46,97,190,26,98,230,15,54,4,45,170,168,4,45,42,187,220,246,162,217,202,
        243,47,130,74,69,222,207,1,10,208,95,174,118,183,155,234,232,90,125,40,240,39,8,133,199,173,127,127,81,191,189,57,191,216,110,174,15,213,94,69,182,202,160,38,89,130,74,212,206,46,53,28,34,65,173,25,192,173,182,98,35,49,101,64,108,44,162,33,5,23,90,128,
        173,86,127,168,159,7,110,60,25,39,209,61,91,63,79,190,97,235,196,113,172,244,101,231,87,237,86,218,152,186,173,86,127,172,205,154,98,27,86,235,226,145,214,249,223,199,7,165,52,6,30,0,195,37,248,73,67,222,82,8,46,141,108,82,130,156,133,195,57,120,165,
        61,1,10,169,73,236,77,136,241,4,136,200,67,65,65,232,36,72,160,174,24,147,151,177,51,197,134,76,226,228,78,129,2,53,148,112,40,161,19,32,24,3,118,180,41,240,41,144,13,141,21,111,77,188,47,104,233,116,37,104,81,79,37,104,81,227,229,198,23,109,87,170,96,
        17,84,42,243,94,94,80,130,230,35,211,62,104,100,158,105,100,26,49,212,38,96,131,51,123,230,97,96,166,134,92,100,118,186,123,235,162,177,209,15,3,211,54,206,5,246,226,219,192,180,49,37,18,121,176,192,124,58,8,76,215,24,103,77,138,78,158,173,125,227,67,
        18,253,133,182,223,191,222,190,193,102,166,40,17,192,140,75,142,170,61,143,23,213,87,188,39,235,83,5,211,140,23,197,54,94,152,133,39,242,99,19,9,90,109,213,218,163,90,249,9,142,25,130,137,193,13,23,109,235,193,72,127,206,225,143,79,201,31,160,16,49,145,
        225,232,246,40,191,92,212,72,73,193,24,242,51,242,17,33,228,147,245,217,27,135,242,25,158,106,61,170,200,179,181,53,77,116,72,222,62,118,242,73,144,205,57,228,60,131,180,13,112,114,210,201,135,194,2,57,138,81,245,15,119,50,54,240,68,190,70,151,4,242,
        34,3,84,43,31,49,236,177,99,118,110,184,104,167,246,93,146,63,68,113,18,84,139,126,255,35,23,161,32,40,229,243,242,11,255,153,141,86,247,208,117,20,17,234,93,48,214,181,60,135,16,64,68,125,184,158,229,112,13,198,167,160,187,183,170,157,148,252,32,90,
        157,146,35,129,74,218,104,21,39,72,152,241,23,138,86,139,228,163,101,92,112,30,129,223,176,3,113,11,99,107,14,80,162,73,8,249,202,117,222,82,44,170,243,37,112,32,215,69,211,96,81,16,202,148,172,184,153,104,69,94,19,120,241,0,213,71,171,210,18,242,60,
        92,60,70,83,106,172,69,238,11,167,228,15,80,164,89,84,108,144,62,90,239,22,163,105,76,64,132,70,158,145,239,184,9,32,69,134,220,92,180,34,125,145,109,163,21,92,52,56,203,125,180,194,200,98,189,215,132,21,64,73,1,12,161,143,86,65,32,146,83,38,43,13,50,
        127,226,224,105,46,90,81,12,157,102,179,2,213,71,171,137,62,58,26,45,218,169,125,231,229,127,57,66,89,142,196,50,99,93,228,105,49,46,166,56,47,189,240,158,217,88,149,113,172,218,79,28,171,172,52,35,136,102,66,43,72,147,6,217,42,148,165,21,121,46,128,
        16,133,108,25,52,27,96,188,142,250,96,205,172,25,159,91,31,89,73,49,130,26,122,182,11,133,245,5,66,245,242,242,226,92,127,215,250,184,241,155,139,75,89,27,120,91,66,23,18,2,62,187,36,253,69,232,226,209,232,111,241,120,143,96,22,247,129,70,21,189,12,168,
        64,219,241,8,122,44,229,58,147,62,165,0,161,81,19,36,171,9,67,43,65,80,76,2,163,128,22,78,128,180,159,67,25,160,144,78,129,144,48,157,122,234,76,131,133,148,97,98,180,39,64,218,170,193,4,54,156,146,148,155,62,5,209,41,16,218,71,139,131,19,223,23,180,
        116,186,18,180,168,167,18,180,168,241,114,227,139,182,43,85,176,8,42,149,121,63,47,40,64,243,145,235,135,145,251,235,154,80,227,242,143,124,178,9,142,66,238,83,222,186,41,207,49,34,158,12,34,130,160,11,2,127,32,117,7,237,122,41,57,166,177,234,75,16,18,
        69,0,57,156,206,37,24,140,82,172,245,246,36,8,213,34,137,120,16,51,208,188,192,160,118,51,150,86,80,108,199,50,247,218,83,1,154,183,79,120,216,193,154,243,231,198,249,159,66,62,38,214,17,147,12,19,18,52,19,158,37,177,113,83,77,244,32,85,124,212,104,157,
        132,24,163,198,6,84,200,112,18,4,78,77,100,116,128,1,230,29,65,145,131,153,54,213,40,106,70,43,254,61,247,84,130,230,173,19,31,214,58,233,226,117,188,224,159,215,58,232,50,200,163,97,178,147,185,95,1,10,220,216,144,144,71,198,244,69,192,80,80,69,179,
        226,193,176,160,226,36,177,163,47,72,155,72,135,168,144,202,33,24,84,207,68,209,201,148,237,236,10,137,33,128,219,40,113,78,100,65,178,102,236,202,198,26,29,2,104,242,3,69,129,188,153,168,67,215,159,103,215,11,167,89,62,242,200,174,63,151,57,169,58,122,
        72,234,237,250,100,194,103,74,123,93,128,84,156,95,148,195,3,156,138,157,201,222,234,144,86,36,89,20,178,130,225,192,59,209,108,122,227,242,58,131,28,39,229,182,119,12,167,16,160,195,20,73,1,133,161,96,56,116,199,112,126,151,25,14,118,177,201,29,200,
        197,70,29,232,167,248,217,227,161,159,149,91,64,13,245,65,71,187,19,138,77,26,213,134,181,62,22,168,125,127,204,94,13,232,169,29,99,145,122,31,162,82,71,110,65,126,129,66,139,19,18,163,191,14,69,195,221,189,220,155,187,151,79,84,88,44,142,229,27,110,
        60,80,146,134,34,218,22,39,180,18,212,131,203,151,219,241,226,242,254,7,168,66,5,60,126,121,169,60,123,82,255,211,100,86,215,123,50,15,60,31,211,1,10,82,123,30,151,184,198,120,147,64,210,123,23,255,125,109,181,133,214,190,70,251,105,214,153,161,14,87,
        6,45,55,154,118,70,163,30,180,41,215,150,27,117,28,84,46,205,243,248,63,29,189,252,242,114,221,63,208,186,227,25,197,227,199,123,62,91,164,36,43,185,255,80,74,65,62,121,55,105,190,129,178,32,125,41,98,247,12,20,154,87,244,112,217,178,103,122,122,171,
        7,214,106,229,149,50,132,40,157,227,248,136,180,139,150,47,14,23,199,142,147,154,104,13,229,137,69,41,226,24,88,172,212,213,90,176,30,112,155,200,228,189,239,123,87,244,87,54,55,85,153,215,10,72,77,30,8,143,3,43,207,127,140,250,158,128,191,34,243,27,
        238,122,123,56,187,4,70,3,25,115,84,4,168,0,213,161,235,46,209,44,163,179,69,139,140,69,189,82,138,50,179,127,228,236,232,116,32,209,162,162,17,72,236,186,87,236,16,205,182,13,202,218,156,52,58,85,50,174,219,255,162,250,231,29,159,30,120,212,20,116,242,
        224,19,114,118,158,34,162,109,128,147,251,34,185,227,67,176,80,177,33,182,39,199,57,197,187,34,185,51,142,239,35,34,27,166,181,214,193,206,17,93,200,47,224,246,163,9,177,105,144,138,216,233,76,69,139,186,183,54,40,75,30,145,6,109,154,24,150,109,81,228,
        29,142,215,77,88,241,65,162,200,94,237,234,161,27,155,151,187,132,201,13,115,66,33,139,121,209,193,251,252,204,76,8,105,79,28,194,42,101,148,231,32,46,246,51,27,104,206,72,240,161,165,53,232,234,28,219,126,194,138,40,116,158,144,69,242,206,208,8,228,
        217,230,140,223,71,239,13,41,202,99,255,160,41,161,159,224,66,4,44,155,179,90,68,110,70,124,128,33,21,242,103,22,39,84,5,145,97,80,41,50,72,4,137,111,142,167,6,23,225,213,54,31,3,239,103,33,63,73,48,202,192,24,57,147,243,121,172,42,246,184,211,179,154,
        213,243,18,54,211,154,193,122,182,253,248,103,209,144,243,1,244,208,119,158,72,31,200,87,129,36,95,125,121,24,148,193,37,203,0,114,141,179,14,231,213,249,15,234,191,179,17,233,163,100,71,122,243,11,39,99,37,29,120,78,1,52,211,127,40,128,138,16,42,34,
        167,127,248,120,102,142,173,226,32,236,244,114,64,251,21,34,47,60,113,240,5,212,113,172,7,143,116,46,5,27,219,163,91,75,137,219,161,234,89,237,242,169,93,202,189,110,22,35,60,33,245,222,54,200,54,240,11,237,80,89,239,134,241,219,230,221,47,107,56,27,
        251,200,89,184,105,34,226,32,117,247,31,209,42,20,65,147,187,109,3,207,69,53,155,220,15,37,4,184,119,49,146,94,113,232,109,63,154,53,246,69,85,74,2,183,204,34,80,1,125,76,49,22,116,79,172,51,217,234,90,47,209,105,207,221,175,160,15,33,145,246,62,87,64,
        27,36,24,62,250,60,18,136,55,150,218,105,49,195,199,19,42,224,81,55,17,137,33,161,25,207,45,2,105,213,160,56,202,93,224,44,40,107,17,154,33,157,103,147,182,251,168,208,157,230,5,205,10,146,186,214,43,149,174,49,25,186,205,91,164,113,44,145,38,189,118,
        34,132,99,79,82,11,235,85,189,69,57,208,61,24,232,151,196,248,254,242,9,245,80,144,191,212,103,83,196,65,208,231,186,78,62,103,130,20,114,69,80,159,128,253,88,75,246,40,82,39,119,160,252,169,39,181,232,16,117,39,46,143,200,197,171,167,74,17,169,9,238,
        11,47,130,11,128,68,32,11,33,55,139,12,57,94,66,124,195,39,148,102,60,207,151,98,193,161,113,188,231,172,182,123,252,148,179,218,122,216,245,232,197,66,55,20,228,187,161,224,216,77,145,126,201,199,60,194,214,76,28,57,223,36,246,23,11,240,46,67,109,223,
        11,237,224,252,254,232,167,234,61,185,124,102,110,134,196,6,41,83,241,38,232,69,98,214,59,66,81,121,152,187,187,197,52,232,7,29,106,135,70,153,215,111,192,32,87,250,226,222,162,27,87,202,210,144,113,241,140,199,250,240,205,249,205,15,155,155,219,127,
        66,57,159,181,207,253,23,214,30,215,87,250,197,178,253,238,118,123,216,238,174,143,227,192,17,232,73,125,147,191,147,118,135,170,245,203,105,189,224,127,205,8,174,235,195,110,255,1,185,79,235,246,187,109,67,193,74,61,255,15,229,51,49,67,0,0 };

        imageFileIcon = LookAndFeel::loadDrawableFromData (data, sizeof (data));
    }

    return imageFileIcon;
}
