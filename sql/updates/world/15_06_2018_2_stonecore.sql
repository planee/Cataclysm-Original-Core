/*Stonecore*/
UPDATE `creature_template` SET `AIName`= '' WHERE `entry`= 43438;
UPDATE `creature` SET `position_x`= 1176.55, `position_y`= 1154.04, `position_z`= 273.974, `orientation`=  5.581 WHERE `guid`= 169209;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_flayer' WHERE `entry`= 42808;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_warbringer' WHERE `entry`= 42696;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_earthshaper' WHERE `entry`= 43537;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_berserker' WHERE `entry`= 43430;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_sentry' WHERE `entry`= 42695;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_magmalord' WHERE `entry`= 42789;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_rift_conjurer' WHERE `entry`= 42691;
UPDATE `creature_template` SET `ScriptName`= 'npc_stonecore_bruiser' WHERE `entry`= 42692;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (81459, 80039);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(81459, 'spell_stonecore_force_of_earth'),
(80039, 'spell_stonecore_magma_eruption_eff');

DELETE FROM `gameobject` WHERE `guid`= 191462;
DELETE FROM `creature_text` WHERE `entry`= 43391;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `BroadcastTextId`, `comment`) VALUES 
(43391, 0, 0, 'Prison taught me one very important lesson, well, two if you count how to hold your soap, but yes! SURVIVAL!', 14, 0, 100, 0, 0, 21789, 46240, ''),
(43391, 1, 0, 'Follow me if you dare!', 14, 0, 100, 0, 0, 21783, 46234, ''),
(43391, 2, 0, 'Ah-ha! I\'ve got you right where I want you!', 14, 0, 100, 0, 0, 21787, 46241, '');

DELETE FROM `locales_creature_text` WHERE `entry`= 43391;
INSERT INTO `locales_creature_text` (`entry`, `groupid`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES 
(43391, 0, 0, '감옥에 있으면서 중요한 걸 하나 배웠어. 아, 밥에서 콩 골라내는 법은 빼고 말이야. 바로 생존이지롱!!!', 'La prison m\'a appris un truc essentiel... heu, deux, si on compte la règle d\'or : ne JAMAIS lâcher la savonnette. Bref, je parle de la SURVIE !', 'Das Gefängnis hat mich eines gelehrt - nun, außer, dass ich meine Seife anständig festhalte: nämlich ÜBERLEBEN!', '监狱生活给我上了一堂很重要的课，如果算上要把肥皂捏稳那就是两堂，是的！生存第一！', '監牢教了我一件很重要的事，好吧，加上把肥皂拿穩算兩件，但沒錯!就是如何生存!', '¡La cárcel me enseñó una lección muy importante, bueno, dos si cuentas cómo sujetar bien el jabón, pero sí! ¡SUPERVIVENCIA!', '¡Si hay algo que aprendí de la cárcel, además de no soltar nunca el jabón, es SOBREVIVIR!', 'В тюрьме я усвоил один очень важный урок. Ну, или два... Первый: держи мыло крепко! Второй: выживай любой ценой!'),
(43391, 1, 0, '따라올 테면 따라와 봐!', 'Suivez-moi si vous l\'osez !', 'Folgt mir, wenn Ihr es wagt!', '够胆子的话就跟我走！', '有膽量就跟上來!', '¡Seguidme si os atrevéis!', '¡O estás con nosotros o en contra! ¡Sígueme si te atreves!', 'Следуйте за мной, если осмелитесь!'),
(43391, 2, 0, '아하! 때려주기 딱 좋은 자리에 섰군!', 'Ha ha ! Exactement où je vous voulais !', 'A-ha! Genau, wo ich Euch haben wollte!', '啊哈！我就是想把你引到这儿来！', '啊哈!我就是要把你引到這!', '¡Ajá! ¡Os tengo justo donde quería! Ahora....', '¡Ajá! ¡Te tengo donde te quería!', 'Ахаха! Вы как раз там, где мне и надо!');
