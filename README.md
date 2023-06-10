# jiffyedit-extra

Extra officially supported plugins for Jiffyedit not found in the base repo.

## Installation:

If using Arch Linux, use the PKGBUILD.

## Voice Detector - clipper;

  Fetch clips by detecting voice via whisper.cpp, in one of 99 languages. A .wav file, which is needed for whisper, is created automatically and saved in the same directory as the video. 

  [vd]
th # - Number of threads to use. Cannot be more than 12. Default number of active CPU threads.

md [ model ] - Model to use. Valid options are [ base, base_en, large, large_v1, medium, medium_en, tiny, tiny_en ]. Default base.

ln [ language ] - Language to subtitle/transcribe in. irrelevant unless in a non-English language and using a non-en model. Default English.

  Availabe languages;

    [   en,       zh,     de,     es,       ru,     ko,     fr,      ja,       pt,        tr,     pl,     ca,      nl,    ar,      sv,      it,       id,      hi,     fi,        vi,       iw,       uk,      el,    ms,    cs,    ro,       da,       hu,     ta,      no,      th,   ur,     hr,       bg,         lt,      la,     mi,     ml,     cy,    sk,     te,     fa,      lv,       bn,      sr,       az,          sl,      kn,       et,        mk,       br,     eu,      is,        hy,      ne,      mn,        bs,     kk,      sq,       sw,      gl,       mr,     pa,       si,    km,    sn,     yo,     so,      af,       oc,       ka,        be,      tg,     sd,     gu,       am,      yi,    lo,   uz,      fo,         ht,         ps,     tk,      nn,      mt,      sa,           lb,        my,       bo,      tl,     mg,        as,      tt,      haw,      ln,     ha,      ba,       jw,         su ]     
    
    [ english, chinese, german, spanish, russian, korean, french, japanese, portuguse, turkish, polish, catalan, dutch, arabic, swedish, italian, indonesian, hindi, finnish, vietnamese, hebrew, ukrainian, greek, malay, czech, romanian, danish, hungarian, tamil, norwegian, thai, urdu, croatian, bulgarian, lithuanian, latin, maori, malayam, welsh, slovak, telugu, persian, latvian, bengali, serbian, azerbaijani, slovenian, kannada, estonian, macedonian, breton, basque, icelandic, armenian, nepali, mongolian, bosnian, kazakh, albanian, swahili, galician, marathi, punjabi, sinhala, khmer, shona, yoruba, somali, afrikaans, occitan, georgian, belarusian, tajik, sindhi, gujarati, amharic, yiddish, lao, uzbek, faroese, haitian creole, pashto, turkmen, nynorsk, maltese, sanskrit, luxembourgish, myanmar, tibetan, tagalog, malagassy, assamese, tatar, hawaiian, lingala, hausa, bashkir, javanese, sundanese ]

bf # - Buffer size betwean clips. Sometimes, parts of the same sentence can be split into different clips. This works around that issue. Default 0.2 seconds.

bt # # - Only clip between these portions of the video.

cm [ model ] - Custom model. Make sure to specify full path.

fp [ font path ] - Specify custom font path. This is only relevant if missing font errors are being thrown (may fail without error message).

aa # - Additional arguments to pass to whisper.cpp.

Example command: [vd], [vd bf 0.5 th 4 md medium_en ln de bt 10 500 fp /home/user/fonts/font aa "-sow -nt" ]

## Roadmap;

- Fix bugs

- Choose new license

- More plugins

## Note:

I will not be accepting any new plugins or contributions to existing plugins until I have chosen a new license.
