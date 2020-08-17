DELETE FROM `creature_text` WHERE `entry` IN (42188, 42333);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `type`, `language`, `probability`, `emote`, `sound`, `comment`, `BroadcastTextId`, `text`) VALUES
(42188, 0, 0, 14, 0, 100, 0, 21919, '', 46247, 'None may pass into the World''s Heart!'),
(42188, 1, 0, 14, 0, 100, 0, 21921, '', 46249, 'Break yourselves upon my body. Feel the strength of the earth!'),
(42188, 2, 0, 41, 0, 100, 0,     0, '', 46010, '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|tOzruk casts |cFFFF0000|Hspell:78939|h[Elementium Bulwark]|h|r!'),
(42188, 3, 0, 14, 0, 100, 0, 21922, '', 46250, 'A protector has fallen. The World''s Heart lies exposed!'),
(42188, 4, 0, 14, 0, 100, 0, 21920, '', 46248, 'The cycle is complete.'),
(42333, 0, 0, 14, 0, 100, 0, 21634, '', 46255, 'The world will be reborn in flames!'),
(42333, 1, 0, 14, 0, 100, 0, 21628, '', 46253, 'Witness the power bestowed upon me by Deathwing! Feel the fury of earth!'),
(42333, 2, 0, 14, 0, 100, 0, 21635, '', 46254, 'A sacrifice for you, master.'),
(42333, 3, 0, 14, 0, 100, 0, 21633, '', 46251, 'For my death, countless more will fall. The burden is now yours to bear.');

DELETE FROM `locales_creature_text` WHERE `entry` IN (42188, 42333);
INSERT INTO `locales_creature_text` (`entry`, `groupid`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(42188, 0, 0, '세계의 심장에는 아무도 들어갈 수 없다!', 'Nul n''entre dans le cœur du monde !', 'Niemand dringt ins Herz der Welt vor!', '没人能接近世界之心！', '沒有人可以穿過世界之心!', '¡Nadie entrará en el corazón del mundo!', '¡No pasarán al Centro del mundo!', 'Никто не пройдет к сердцу мира!'),
(42188, 1, 0, '내 몸에 부딪히고, 부서져라! 대지의 강력함을 느껴 보아라!', 'Venez vous briser sur mon corps. Sentez la force de la terre !', 'Ihr werdet an meinem Körper zerschellen. Spürt der Erde Macht!', '被我的身躯碾碎。感受大地的力量！', '以我的身軀摧毀你們。感受大地之力!', 'Someteos a mi cuerpo. ¡Sentid la Fuerza de la tierra!', 'Quiébrate sobre mi cuerpo. ¡Siente la fuerza de la tierra!', 'Разбейтесь о меня. Почувствуйте силу земли!'),
(42188, 2, 0, '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t오즈룩이 |cFFFF0000|Hspell:78939|h[엘레멘티움 보루방패]|h|r를 시전합니다!', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|tOzruk lance |cFFFF0000|Hspell:78939|h[Rempart d''élémentium]|h|r !', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t Ozruk wirkt |cFFFF0000|Hspell:78939|h[Elementiumbollwerk]|h|r!', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t欧泽鲁克开始施放|cFFFF0000|Hspell:78939|h[源质壁垒]|h|r！', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t 歐茲魯克施放了|cFFFF0000|Hspell:78939|h[源質壁壘]|h|r!', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t¡Ozruk lanza |cFFFF0000|Hspell:78939|h[Baluarte de elementium]|h|r!', '|TInterfaceIconsability_warrior_shieldreflection.blp:20|t¡Ozruk lanza |cFFFF0000|Hspell:78939|h[Baluarte de elementium]|h|r!', '|TInterface\Icons\ability_warrior_shieldreflection.blp:20|t Озрук накладывает заклинание |cFFFF0000|Hspell:78939|h[Элементиевая защита]|h|r!'),
(42188, 3, 0, '수호자가 쓰러졌다. 세계의 심장이 위험에 노출됐다!', 'Un protecteur est tombé. Le cœur du monde est vulnérable !', 'Ein Beschützer ist gefallen. Das Herz der Welt ist entblößt!', '保护者倒下了。世界之心暴露了！', '一位保護者倒下了。世界之心有危險了!', 'Un protector ha caído. ¡El corazón del mundo está expuesto!', 'Ha caído un protector. ¡El Centro del mundo, expuesto!', 'Хранитель повержен... Сердце мира беззащитно!'),
(42188, 4, 0, '생명의 순환이 완성되었다.', 'Le cycle est terminé.', 'Der Zyklus ist abgeschlossen.', '轮回完成了。', '輪迴已完成。', 'El ciclo está completo.', 'Se completó el ciclo.', 'Цикл завершен.'),
(42333, 0, 0, '세상은 불꽃 속에서 다시 태어나리라!', 'Le monde va renaître dans les flammes !', 'Die Welt wird aus Flammen neu entstehen!', '这个世界将在烈焰中重生！', '世界會在火焰中重生!', '¡El mundo renacerá entre llamas!', '¡El mundo renacerá en llamas!', 'Мир переродится в пламени!'),
(42333, 1, 0, '데스윙 님께서 내게 허락하신 힘을 보아라! 대지의 분노를 느껴라!', 'Voyez le pouvoir que m''a conféré Aile de mort ! Sentez la fureur du monde !', 'Spürt die Macht, die Todesschwinge mir verlieh! Spürt den rasenden Zorn der Erde!', '见证死亡之翼赋予我的强大力量吧！感受大地之怒！', '見證死亡之翼賜予的力量吧!感受大地之怒!', '¡Observad el poder que me ha otorgado Alamuerte! ¡Sentid la furia de la tierra!', '¡Siente el poder de Alamuerte en mí! ¡La furia de la tierra!', 'Узрите мощь, дарованную мне Смертокрылом! Почувствуйте ярость земли!'),
(42333, 2, 0, '당신께 바칩니다, 주인님.', 'Un sacrifice pour vous, maître.', 'Ein Opfer für Euch, Meister.', '主人，请收下祭品。', '收下祭品吧，主人。', 'Un sacrificio para ti, maestro.', 'Un sacrificio para usted, amo.', 'Жертва для тебя, господин.'),
(42333, 3, 0, '나의 죽음 때문에 셀 수 없이 많은 이가 목숨을 잃을 것이다. 다... 너희의 책임이야...', 'Ils seront innombrables à tomber pour ma mort. Ce poids sera sur vos épaules.', 'Meinem Tod werden Unzählige folgen. Diese Last müsst Ihr nun tragen.', '无数人将随我的死亡而倒下。重担……落到你们……肩上了。', '我的死亡只會帶來更多的死亡。這個重擔現在換你來背了。', 'Tras mi muerte, muchos más caerán. Ahora vosotros llevaréis la carga.', 'Tras mi muerte, habrá infinitas más. El peso está en tus hombros ahora.', 'Моя гибель откроет бесконечную череду смертей. Мое бремя ложится на вас.');

INSERT IGNORE INTO `creature_text` (`entry`, `groupid`, `id`, `type`, `language`, `probability`, `emote`, `sound`, `comment`, `BroadcastTextId`, `text`) VALUES
(43391, 3, 0, 14, 0, 100, 0, 21788, '', 46242, 'Now... witness the full power of Millhouse Manastorm!');

INSERT IGNORE INTO `locales_creature_text` (`entry`, `groupid`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(43391, 3, 0, '자... 밀하우스 마나스톰 님의 진정한 힘을 보아라!', 'Bien... Admirez toute la puissance de Milhouse Tempête-de-Mana !', 'Nun spürt die ganze Macht von Millhaus Manasturm!', '现在……见识米尔豪斯·法力风暴的全部力量吧！', '現在...見證米歐浩斯·曼納斯頓完全的力量吧!', '¡Contemplad el poder de Molino Tormenta de Maná!', '¡Siente el poder del Molino Tormenta de Maná!', 'А теперь... Узрите всю мощь Миллхауса Манашторма!');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (81008, 92631);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(81008, 'spell_stonecore_quake'),
(92631, 'spell_stonecore_quake');