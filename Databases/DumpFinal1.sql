-- MySQL dump 10.14  Distrib 5.5.57-MariaDB, for Win64 (AMD64)
--
-- Host: localhost    Database: ourfitnessdb
-- ------------------------------------------------------
-- Server version	5.5.57-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Temporary table structure for view `all_persons'_blood_tests`
--

DROP SCHEMA IF EXISTS `ourfitnessdb`;
CREATE SCHEMA `ourfitnessdb`;
USE `ourfitnessdb`;

DROP TABLE IF EXISTS `all_persons'_blood_tests`;
/*!50001 DROP VIEW IF EXISTS `all_persons'_blood_tests`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `all_persons'_blood_tests` (
  `PersonID` tinyint NOT NULL,
  `PersonName` tinyint NOT NULL,
  `Surname` tinyint NOT NULL,
  `Iron` tinyint NOT NULL,
  `Cholesterol` tinyint NOT NULL,
  `Blood_sugar` tinyint NOT NULL,
  `Date` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Temporary table structure for view `all_persons_illnesses_forbidden_foods`
--

DROP TABLE IF EXISTS `all_persons_illnesses_forbidden_foods`;
/*!50001 DROP VIEW IF EXISTS `all_persons_illnesses_forbidden_foods`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `all_persons_illnesses_forbidden_foods` (
  `PersonID` tinyint NOT NULL,
  `PersonName` tinyint NOT NULL,
  `PersonSurname` tinyint NOT NULL,
  `Illness` tinyint NOT NULL,
  `Food` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `blood_test`
--

DROP TABLE IF EXISTS `blood_test`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `blood_test` (
  `Date` date NOT NULL,
  `Iron` int(10) unsigned DEFAULT NULL,
  `Cholesterol` int(10) unsigned DEFAULT NULL,
  `LDL` int(10) unsigned DEFAULT NULL,
  `HDL` int(10) unsigned DEFAULT NULL,
  `Blood_sugar` int(10) unsigned DEFAULT NULL,
  `PersonID` int(11) NOT NULL,
  PRIMARY KEY (`Date`,`PersonID`),
  KEY `fk_Blood_test_Person1_idx` (`PersonID`),
  CONSTRAINT `fkPersonID` FOREIGN KEY (`PersonID`) REFERENCES `person` (`PersonID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `blood_test`
--

LOCK TABLES `blood_test` WRITE;
/*!40000 ALTER TABLE `blood_test` DISABLE KEYS */;
INSERT INTO `blood_test` VALUES ('2015-05-05',100,170,110,90,95,1),('2018-02-03',80,260,169,80,90,5),('2018-04-06',112,160,150,100,80,4),('2019-04-10',87,200,178,43,75,3),('2019-07-07',100,290,160,150,130,7),('2020-08-06',120,150,110,90,100,6),('2020-11-11',140,180,110,100,100,3);
/*!40000 ALTER TABLE `blood_test` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `diet_plan`
--

DROP TABLE IF EXISTS `diet_plan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `diet_plan` (
  `dietID` int(11) NOT NULL,
  `Breakfast` varchar(35) DEFAULT NULL,
  `Lunch` varchar(35) DEFAULT NULL,
  `Dinner` varchar(35) DEFAULT NULL,
  `Snack` varchar(35) DEFAULT NULL,
  `Weekday` enum('Mon','Tue','Wed','Thu','Fri','Sat','Sun') DEFAULT NULL,
  `PersonID` int(11) NOT NULL,
  PRIMARY KEY (`dietID`),
  KEY `fk_Diet_plan_Person1_idx` (`PersonID`),
  CONSTRAINT `fk_Diet_plan_Person1` FOREIGN KEY (`PersonID`) REFERENCES `person` (`PersonID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `diet_plan`
--

LOCK TABLES `diet_plan` WRITE;
/*!40000 ALTER TABLE `diet_plan` DISABLE KEYS */;
INSERT INTO `diet_plan` VALUES (1,'Milk with cereals','Chicken with potatoes','Rice with beef','Yoghurt with honey','Fri',1),(2,'Yoghurt with honey','Macaroni with minced beef','Potato souvlaki with mustard sauce','Yoghurt with honey','Tue',1),(3,'Pancakes','Chicken with rice','Potato souvlaki with mustard sauce','Protein shake','Mon',2),(4,'Milk with cereals','Chicken with potatoes','Pancakes','Yoghurt with honey','Fri',3),(5,'Pancakes','Chicken with rice','Macaroni with minced beef','Milk with cereals','Sat',5),(6,'Pancakes','Macaroni with minced beef','Macaroni with minced beef','Pancakes','Mon',7);
/*!40000 ALTER TABLE `diet_plan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Temporary table structure for view `diet_plans_based_on_target`
--

DROP TABLE IF EXISTS `diet_plans_based_on_target`;
/*!50001 DROP VIEW IF EXISTS `diet_plans_based_on_target`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8;
/*!50001 CREATE TABLE `diet_plans_based_on_target` (
  `Target_Type` tinyint NOT NULL,
  `Weight_change` tinyint NOT NULL,
  `Breakfast` tinyint NOT NULL,
  `Lunch` tinyint NOT NULL,
  `Dinner` tinyint NOT NULL,
  `Snack` tinyint NOT NULL,
  `DietID` tinyint NOT NULL
) ENGINE=MyISAM */;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `exercise_plan`
--

DROP TABLE IF EXISTS `exercise_plan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `exercise_plan` (
  `exID` enum('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15') NOT NULL,
  `Type` enum('Strength','Cardio','HIIT') DEFAULT NULL,
  `Frequency` enum('3','4','5') DEFAULT NULL,
  `Duration` enum('25','30','45','60') DEFAULT NULL,
  PRIMARY KEY (`exID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `exercise_plan`
--

LOCK TABLES `exercise_plan` WRITE;
/*!40000 ALTER TABLE `exercise_plan` DISABLE KEYS */;
INSERT INTO `exercise_plan` VALUES ('1','Strength','4','60'),('2','Strength','5','45'),('3','Cardio','4','30'),('4','Cardio','3','45'),('5','HIIT','3','30'),('6','HIIT','4','45'),('7','Strength','4','45'),('8','Strength','5','25');
/*!40000 ALTER TABLE `exercise_plan` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `OurFitnessDB`.`Exercise_plan_BEFORE_INSERT` BEFORE INSERT ON `Exercise_plan` FOR EACH ROW
BEGIN

END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `food`
--

DROP TABLE IF EXISTS `food`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `food` (
  `Food_name` varchar(35) NOT NULL,
  `Category` enum('Nut','Pasta','Meat','Vegetables','Legume','Dairy') DEFAULT NULL,
  `Carbohydrates` int(10) unsigned DEFAULT NULL,
  `Protein` int(10) unsigned DEFAULT NULL,
  `Fat` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`Food_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `food`
--

LOCK TABLES `food` WRITE;
/*!40000 ALTER TABLE `food` DISABLE KEYS */;
INSERT INTO `food` VALUES ('Almonds','Nut',22,21,50),('Bean','Legume',63,21,1),('Beef','Meat',0,26,15),('Chicken','Meat',0,30,1),('Feta cheese','Dairy',0,15,23),('Lettuce','Vegetables',3,1,0),('Macaroni ','Pasta',75,13,1),('Milk','Dairy',5,5,3),('Potato','Vegetables',17,2,0),('Rice','Vegetables',55,10,0),('Tomato','Vegetables',4,1,0),('Whey Protein','Dairy',5,85,1),('Yoghurt','Dairy',0,17,5);
/*!40000 ALTER TABLE `food` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `food_not_allowed_at_illness`
--

DROP TABLE IF EXISTS `food_not_allowed_at_illness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `food_not_allowed_at_illness` (
  `Illness_name` varchar(35) NOT NULL,
  `Food_name` varchar(35) NOT NULL,
  PRIMARY KEY (`Illness_name`,`Food_name`),
  KEY `fk_Illness_has_Food_Food1_idx` (`Food_name`),
  KEY `fk_Illness_has_Food_Illness1_idx` (`Illness_name`),
  CONSTRAINT `fk_Illness_has_Food_Food1` FOREIGN KEY (`Food_name`) REFERENCES `food` (`Food_name`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Illness_has_Food_Illness1` FOREIGN KEY (`Illness_name`) REFERENCES `illness` (`Illness_name`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `food_not_allowed_at_illness`
--

LOCK TABLES `food_not_allowed_at_illness` WRITE;
/*!40000 ALTER TABLE `food_not_allowed_at_illness` DISABLE KEYS */;
INSERT INTO `food_not_allowed_at_illness` VALUES ('High Blood Pressure','Almonds'),('High Blood Pressure','Feta Cheese'),('High Cholesterol','Beef'),('High Cholesterol','Feta cheese');
/*!40000 ALTER TABLE `food_not_allowed_at_illness` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `illness`
--

DROP TABLE IF EXISTS `illness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `illness` (
  `Illness_name` varchar(35) NOT NULL,
  PRIMARY KEY (`Illness_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `illness`
--

LOCK TABLES `illness` WRITE;
/*!40000 ALTER TABLE `illness` DISABLE KEYS */;
INSERT INTO `illness` VALUES ('Arthritis'),('Asthma'),('Bulimia'),('Diabetes'),('High Blood Pressure'),('High Cholesterol');
/*!40000 ALTER TABLE `illness` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person`
--

DROP TABLE IF EXISTS `person`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person` (
  `PersonID` int(11) NOT NULL,
  `Name` varchar(20) DEFAULT NULL,
  `Surname` varchar(20) DEFAULT NULL,
  `Age` int(10) unsigned DEFAULT NULL,
  `Height` int(10) unsigned DEFAULT NULL,
  `Weight` int(10) unsigned DEFAULT NULL,
  `Has_target` tinyint(1) DEFAULT NULL,
  `Has_blood_test` tinyint(1) DEFAULT NULL,
  `Has_illness` tinyint(1) DEFAULT NULL,
  `TargetID` enum('1','2','3','4','5') NOT NULL,
  `exID` enum('1','2','3','4','5') NOT NULL,
  PRIMARY KEY (`PersonID`),
  KEY `fk_Person_Target_idx` (`TargetID`),
  KEY `fk_Person_Exercise_plan1_idx` (`exID`),
  CONSTRAINT `fkExID` FOREIGN KEY (`exID`) REFERENCES `exercise_plan` (`exID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fkTarget` FOREIGN KEY (`TargetID`) REFERENCES `target` (`TargetID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person`
--

LOCK TABLES `person` WRITE;
/*!40000 ALTER TABLE `person` DISABLE KEYS */;
INSERT INTO `person` VALUES (1,'Giorgos','Papadopoulos',25,185,90,1,1,0,'2','1'),(2,'Napoleon','Papoutsakis',22,178,68,1,0,1,'5','5'),(3,'Kostas','Tsompos',60,172,75,1,1,1,'1','3'),(4,'Nikh','Riga',30,170,55,1,1,0,'3','2'),(5,'Stelios','Steliou',35,179,80,1,1,1,'4','1'),(6,'Takhs','Lemonis',70,176,120,1,0,1,'4','3'),(7,'Evangelos','Marinakis',60,160,160,1,1,1,'4','3');
/*!40000 ALTER TABLE `person` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `person_has_illness`
--

DROP TABLE IF EXISTS `person_has_illness`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `person_has_illness` (
  `PersonID` int(11) NOT NULL,
  `Illness_name` varchar(35) NOT NULL,
  PRIMARY KEY (`PersonID`,`Illness_name`),
  KEY `fk_Person_has_Illness_Illness1_idx` (`Illness_name`),
  KEY `fk_Person_has_Illness_Person1_idx` (`PersonID`),
  CONSTRAINT `fk_Person_has_Illness_Illness1` FOREIGN KEY (`Illness_name`) REFERENCES `illness` (`Illness_name`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Person_has_Illness_Person1` FOREIGN KEY (`PersonID`) REFERENCES `person` (`PersonID`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `person_has_illness`
--

LOCK TABLES `person_has_illness` WRITE;
/*!40000 ALTER TABLE `person_has_illness` DISABLE KEYS */;
INSERT INTO `person_has_illness` VALUES (2,'Asthma'),(3,'Bulimia'),(5,'High Cholesterol'),(6,'Arthritis'),(7,'High Cholesterol');
/*!40000 ALTER TABLE `person_has_illness` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `recipe`
--

DROP TABLE IF EXISTS `recipe`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recipe` (
  `Recipe_name` varchar(35) NOT NULL,
  `Video_link` varchar(100) DEFAULT NULL,
  `Suggested_time` enum('Breakfast','Lunch','Snack','Dinner') DEFAULT NULL,
  `Calories/100g` int(10) unsigned DEFAULT NULL,
  `Category` enum('High_Protein','Low_Fat','Vegeterian','Vegan','Other') DEFAULT NULL,
  PRIMARY KEY (`Recipe_name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `recipe`
--

LOCK TABLES `recipe` WRITE;
/*!40000 ALTER TABLE `recipe` DISABLE KEYS */;
INSERT INTO `recipe` VALUES ('Chicken with potatoes','https://www.youtube.com/watch?v=ri9JmdddIaBgCv','Lunch',150,'High_Protein'),('Macaroni with minced beef','www.youtube.com/sasdfasdf','Lunch',200,'High_Protein'),('Milk with cereals','https://www.youtube.com/watch?v=ri9JmasdfgCv','Breakfast',100,'Other'),('Potato souvlaki with mustard sauce','https://www.youtube.com/watch?v=ri9JmIaBgCv','Dinner',100,'Vegan'),('Rice with beef','https://www.youtube.com/watch?v=ri9JmIaBgCvfff','Lunch',200,'High_Protein'),('Tomatorice','https://www.youtube.com/watch?v=ttt555rrr','Lunch',150,'Vegeterian'),('Yoghurt with honey','https://www.youtube.com/watch?v=riasdfIadsfCv','Snack',100,'Low_Fat'),('Yoghurt with Whey Protein','https://www.youtube.com/watch?fedg','Snack',100,'High_Protein');
/*!40000 ALTER TABLE `recipe` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `recipe_belongs_to_diet_plan`
--

DROP TABLE IF EXISTS `recipe_belongs_to_diet_plan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recipe_belongs_to_diet_plan` (
  `dietID` int(11) NOT NULL,
  `Recipe_name` varchar(35) NOT NULL,
  `Serving(grams)` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`dietID`,`Recipe_name`),
  KEY `fk_Diet_plan_has_Recipe_Recipe1_idx` (`Recipe_name`),
  KEY `fk_Diet_plan_has_Recipe_Diet_plan1_idx` (`dietID`),
  CONSTRAINT `fk_Diet_plan_has_Recipe_Diet_plan1` FOREIGN KEY (`dietID`) REFERENCES `diet_plan` (`dietID`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Diet_plan_has_Recipe_Recipe1` FOREIGN KEY (`Recipe_name`) REFERENCES `recipe` (`Recipe_name`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `recipe_belongs_to_diet_plan`
--

LOCK TABLES `recipe_belongs_to_diet_plan` WRITE;
/*!40000 ALTER TABLE `recipe_belongs_to_diet_plan` DISABLE KEYS */;
INSERT INTO `recipe_belongs_to_diet_plan` VALUES (1,'Chicken with potatoes',350),(1,'Milk with cereals',280),(1,'Rice with beef',300),(2,'Yoghurt with honey',280),(3,'Potato souvlaki with mustard sauce',300),(4,'Milk with cereals',200),(4,'Yoghurt with honey',170);
/*!40000 ALTER TABLE `recipe_belongs_to_diet_plan` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `recipe_contains_food`
--

DROP TABLE IF EXISTS `recipe_contains_food`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `recipe_contains_food` (
  `Recipe_name` varchar(35) NOT NULL,
  `Food_name` varchar(35) NOT NULL,
  `Quantity` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`Recipe_name`,`Food_name`),
  KEY `fk_Recipe_has_Food_Food1_idx` (`Food_name`),
  KEY `fk_Recipe_has_Food_Recipe1_idx` (`Recipe_name`),
  CONSTRAINT `fk_Recipe_has_Food_Food1` FOREIGN KEY (`Food_name`) REFERENCES `food` (`Food_name`) ON DELETE NO ACTION ON UPDATE NO ACTION,
  CONSTRAINT `fk_Recipe_has_Food_Recipe1` FOREIGN KEY (`Recipe_name`) REFERENCES `recipe` (`Recipe_name`) ON DELETE NO ACTION ON UPDATE NO ACTION
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `recipe_contains_food`
--

LOCK TABLES `recipe_contains_food` WRITE;
/*!40000 ALTER TABLE `recipe_contains_food` DISABLE KEYS */;
INSERT INTO `recipe_contains_food` VALUES ('Chicken with potatoes','Chicken',250),('Macaroni with minced beef','Macaroni',250),('Milk with cereals','Milk',250),('Potato souvlaki with mustard sauce','Potato',300),('Rice with beef','Beef',250),('Rice with beef','Rice',150),('Tomatorice','Tomato',200),('Yoghurt with honey','Yoghurt',250),('Yoghurt with Whey Protein','Whey Protein',20),('Yoghurt with Whey Protein','Yoghurt',200);
/*!40000 ALTER TABLE `recipe_contains_food` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `target`
--

DROP TABLE IF EXISTS `target`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `target` (
  `TargetID` enum('1','2','3','4','5') NOT NULL,
  `Type` enum('Increase','Decrease','Maintain') DEFAULT NULL,
  `Weight_change` enum('0','0.25','0.5') DEFAULT NULL,
  PRIMARY KEY (`TargetID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `target`
--

LOCK TABLES `target` WRITE;
/*!40000 ALTER TABLE `target` DISABLE KEYS */;
INSERT INTO `target` VALUES ('1','Increase','0.25'),('2','Increase','0.5'),('3','Decrease','0.25'),('4','Decrease','0.5'),('5','Maintain','0');
/*!40000 ALTER TABLE `target` ENABLE KEYS */;
UNLOCK TABLES;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `OurFitnessDB`.`Target_BEFORE_INSERT` BEFORE INSERT ON `Target` FOR EACH ROW
BEGIN
	IF (NEW.Weight_change != '0') AND (NEW.Type = 'Maintain') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;

	IF (NEW.Weight_change = '0') AND (NEW.Type = 'Increase') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;
    
    IF (NEW.Weight_change = '0') AND (NEW.Type = 'Decrease') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;
    
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `OurFitnessDB`.`Target_BEFORE_UPDATE` BEFORE UPDATE ON `Target` FOR EACH ROW

BEGIN
	IF (NEW.Weight_change != '0') AND (NEW.Type = 'Maintain') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;

	IF (NEW.Weight_change = '0') AND (NEW.Type = 'Increase') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;
    
    IF (NEW.Weight_change = '0') AND (NEW.Type = 'Decrease') THEN
    SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'NONSENSE DATA';
    END IF;
    
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Final view structure for view `all_persons'_blood_tests`
--

/*!50001 DROP TABLE IF EXISTS `all_persons'_blood_tests`*/;
/*!50001 DROP VIEW IF EXISTS `all_persons'_blood_tests`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `all_persons'_blood_tests` AS select `person`.`PersonID` AS `PersonID`,`person`.`Name` AS `PersonName`,`person`.`Surname` AS `Surname`,`blood_test`.`Iron` AS `Iron`,`blood_test`.`Cholesterol` AS `Cholesterol`,`blood_test`.`Blood_sugar` AS `Blood_sugar`,`blood_test`.`Date` AS `Date` from (`person` join `blood_test` on((`blood_test`.`PersonID` = `person`.`PersonID`))) where (`person`.`Has_blood_test` = 1) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `all_persons_illnesses_forbidden_foods`
--

/*!50001 DROP TABLE IF EXISTS `all_persons_illnesses_forbidden_foods`*/;
/*!50001 DROP VIEW IF EXISTS `all_persons_illnesses_forbidden_foods`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `all_persons_illnesses_forbidden_foods` AS select `person`.`PersonID` AS `PersonID`,`person`.`Name` AS `PersonName`,`person`.`Surname` AS `PersonSurname`,`person_has_illness`.`Illness_name` AS `Illness`,`food_not_allowed_at_illness`.`Food_name` AS `Food` from ((`person` join `person_has_illness` on((`person`.`PersonID` = `person_has_illness`.`PersonID`))) join `food_not_allowed_at_illness` on((`food_not_allowed_at_illness`.`Illness_name` = `person_has_illness`.`Illness_name`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `diet_plans_based_on_target`
--

/*!50001 DROP TABLE IF EXISTS `diet_plans_based_on_target`*/;
/*!50001 DROP VIEW IF EXISTS `diet_plans_based_on_target`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8 */;
/*!50001 SET character_set_results     = utf8 */;
/*!50001 SET collation_connection      = utf8_general_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `diet_plans_based_on_target` AS select `target`.`Type` AS `Target_Type`,`target`.`Weight_change` AS `Weight_change`,`diet_plan`.`Breakfast` AS `Breakfast`,`diet_plan`.`Lunch` AS `Lunch`,`diet_plan`.`Dinner` AS `Dinner`,`diet_plan`.`Snack` AS `Snack`,`diet_plan`.`dietID` AS `DietID` from ((`person` join `target` on((`person`.`TargetID` = `target`.`TargetID`))) join `diet_plan` on((`person`.`PersonID` = `diet_plan`.`PersonID`))) */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2020-12-19 22:55:51
