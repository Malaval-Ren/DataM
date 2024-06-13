/*
 *  pathname.h
 *  Iron Lord : tous les fichiers
 *
 * Created by Renaud Malaval on 2022-09-22.
 * Copyright © 2022 Renaud Malaval & Frédéric Mure. All rights reserved.
 */

#ifndef __IL_PATHNAME__
#define __IL_PATHNAME__

/****************************************************************
 * Images files name
 ***************************************************************/

// #define PICTURE_FOLDER "Dessin:"

enum ePicFileName {
    eIntroPic,
    eCardPic, eHorsePic, eLetterPic,
    eEntreAbbayePic, eAbbayePic, eCarteAbbaye,
    eEntreMalabryPic, eMalabryPic, eCarteMalabry,
    eEntreCastlePic, eCastlePic, eCarteCastle,
    eEntreKorandoPic, eKorandoPic, eKorandoCastle,
    eEntreTorentekPic, eTorentekPic, eTorentekCastle,
    eEntreGrinderPic, eGrinderPic,      /* Meunier */
    eEntreSorcierPic, eSorcierPic,      /* Sorcier*/
    eCadrePic, eArcherPic, eTirPic, eSetArc2Pic, eSetArc1Pic, eCiblesPic,   /* Tir a l'arc */
    eDes2, eDes, eSprDes,               /* Jeux de Des */
    eSol, eCrane,                       /* Labyrinthe */
    eGamesRoomPic,                      /* Taverne */
    eHerboriestPic,
    eAubergistePic,
    eMarchandPic,
    eMercenairePic,
    eMoinePic,
    eServantePic,
    eTemplierPic,
    eMeditePic
};



static const char *pIntroPicture           = "Dessin:iron.lord.pic";

static const char *pCardPicture            = "Dessin:carte.pic";
static const char *pHorsePicture           = "Dessin:chevaux3.pic";
static const char *pLetterPicture          = "Dessin:letters.pic";

static const char *pEntreAbbayePicture     = "Dessin:abbaye.ch.pic";
static const char *pAbbayePicture          = "Dessin:abbaye.che.pic";
static const char *pCarteAbbayePicture     = "Dessin:abbaye.map.pic";

static const char *pEntreMalabryPicture    = "Dessin:malabr.ch.pic";
static const char *pMalabryPicture         = "Dessin:malabr.che.pic";
static const char *pCarteMalabryPicture    = "Dessin:malabr.map.pic";

static const char *pEntreCastlePicture     = "Dessin:castle.ch.pic";
static const char *pCastlePicture          = "Dessin:castle.che.pic";
static const char *pCarteCastlePicture     = "Dessin:castle.map.pic";

static const char *pEntreKorandoPicture    = "Dessin:korand.ch.pic";
static const char *pKorandoPicture         = "Dessin:korand.che.pic";
static const char *pCarteKorandoPicture    = "Dessin:korand.map.pic";

static const char *pEntreTorentekPicture   = "Dessin:torent.ch.pic";
static const char *pTorentekPicture        = "Dessin:torent.che.pic";
static const char *pCarteTorentekPicture   = "Dessin:torent.map.pic";

static const char* pEntreGrinderPicture     = "Dessin:meunie.ch.pic";
static const char* pGrinderPicture          = "Dessin:meunie.che.pic";

static const char *pEntreSorcierPicture    = "Dessin:sorcie.ch.pic";
static const char *pSorcierPicture         = "Dessin:sorcie.che.pic";

// all these paths are used for archery games
static const char *pCadrePicture           = "Dessin:cadre.pic";
static const char *pArcherPicture          = "Dessin:archer.pic";
static const char *pTirPicture             = "Dessin:tir.pic";
static const char *pSetArc2Picture         = "Dessin:setarc2.pic";
static const char *pSetArc1Picture         = "Dessin:setarc1.pic";
static const char *pCiblesPicture          = "Dessin:cibles.pic";

static const char *pDes2                   = "Dessin:des.2.pic";
static const char *pDes                    = "Dessin:des.pic";
static const char *pSprDes                 = "Dessin:spr.des.pic";

static const char *pSol                    = "Dessin:sol.pic";
static const char *pCrane                  = "Dessin:crane.pic";

/*
// Don't delete this commented code its for image.sh script
// all this pathame are re-used in pMarchand defined in human.cc file
static const char *pGamesRoomPicture       = "Dessin:taverne.pic";
static const char *pHerboriestPicture      = "Dessin:herbor.ch.pic";
static const char *pAubergistePicture      = "Dessin:auberg.ch.pic";
static const char *pMarchandPicture        = "Dessin:marcha.ch.pic";
static const char *pMercenairePicture      = "Dessin:mercen.ch.pic";
static const char *pMoinePicture           = "Dessin:moine.ch.pic";
static const char *pServantePicture        = "Dessin:servan.ch.pic";
static const char *pTemplierPicture        = "Dessin:templi.ch.pic";
static const char *pMeditePicture          = "Dessin:medite.ch.pic";
*/

static const char* pAbbaye_start01_en       = "The abbey of the Templars.\nIn peace time of a place of meditation.";
static const char* pAbbaye_start02_en       = "You are in the garden of abbey, where the templars come to rest.";
static const char* pMalabry_start01_en      = "You're in the little village of Chatenay Malabry, where the annual archery contest is taking place.";
static const char* pMalabry_start02_en      = "The village of Chatenay Malabry.\nIn the distance you can heart the sound of the archery contest.";
static const char* pCastle_start01_en       = "Your ancestral castle.\nIt is now uninhabited and almost derelict.";
static const char* pKorando_start01_en      = "The village of Korando.\nThere is an inn here, and a salesman of ill repute.";
static const char* pKorando_start02_en      = "Marchand and Aubergist...";
static const char* pGrinder_menu01_en[7]    = { "Inventory","Examine","Discuss","Buy","Give","Money","Leave" };
static const char* pTorentek_start01_en     = "The bustling town of Torentek.\nThis place is crawling with thieves and murderers.";
static const char* pTorentek_start02_en     = "You are near the ruins of an old church. Nobody will be surprised the day its walls collapse.";
static const char* pSorcier_menu01_en[7]    = { "Inventory","Examine","Discuss","Buy","Give","Money","Leave" };

static const char *pTaverne_start01_en      = "You are in the gambling hall, where an arm-wrestling context is taking peace. A rich captain is is challenging people to a game of dice.";

// static const char *pWarBlasonsPicture      = "Dessin:Blasons.pic";
// static const char *pWarTerrainPicture      = "Dessin:Terrain.pic";

#endif
