-- MySQL dump 10.13  Distrib 8.0.24, for Win64 (x86_64)
--
-- Host: localhost    Database: covidbus
-- ------------------------------------------------------
-- Server version	8.0.24

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `actuador`
--

DROP TABLE IF EXISTS `actuador`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `actuador` (
  `idactuador` int NOT NULL AUTO_INCREMENT,
  `tipo` varchar(45) NOT NULL,
  `nombre` varchar(45) NOT NULL,
  `iddispositivo` int NOT NULL,
  PRIMARY KEY (`idactuador`),
  KEY `dispositivo_idx` (`iddispositivo`),
  CONSTRAINT `id_dispositivo` FOREIGN KEY (`iddispositivo`) REFERENCES `dispositivo` (`iddispositivo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `actuador`
--

LOCK TABLES `actuador` WRITE;
/*!40000 ALTER TABLE `actuador` DISABLE KEYS */;
INSERT INTO `actuador` VALUES (1,'led','ledbus1',1),(2,'sonido','sonidobus1',1),(3,'led','ledbus2',2),(4,'sonido','sonidobus2',2);
/*!40000 ALTER TABLE `actuador` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `data_sensor`
--

DROP TABLE IF EXISTS `data_sensor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `data_sensor` (
  `timestamp` varchar(50) NOT NULL,
  `valor1` float NOT NULL,
  `valor2` float NOT NULL,
  `idsensor` int NOT NULL,
  PRIMARY KEY (`timestamp`),
  KEY `idsensor_idx` (`idsensor`),
  CONSTRAINT `iddsensor` FOREIGN KEY (`idsensor`) REFERENCES `info_sensor` (`idsensor`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `data_sensor`
--

LOCK TABLES `data_sensor` WRITE;
/*!40000 ALTER TABLE `data_sensor` DISABLE KEYS */;
INSERT INTO `data_sensor` VALUES ('21:04:10_May 24 2021',25.6,37,77),('21:24:33_May 24 2021',23.9,41,77),('21:24:40_May 24 2021',61,61,77);
/*!40000 ALTER TABLE `data_sensor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dispositivo`
--

DROP TABLE IF EXISTS `dispositivo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `dispositivo` (
  `iddispositivo` int NOT NULL AUTO_INCREMENT,
  `autobus` varchar(45) NOT NULL,
  `idusuario` int NOT NULL,
  PRIMARY KEY (`iddispositivo`),
  KEY `usuario_idx` (`idusuario`),
  CONSTRAINT `usuario` FOREIGN KEY (`idusuario`) REFERENCES `usuario` (`idusuario`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=25 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dispositivo`
--

LOCK TABLES `dispositivo` WRITE;
/*!40000 ALTER TABLE `dispositivo` DISABLE KEYS */;
INSERT INTO `dispositivo` VALUES (1,'bus1_put',1),(2,'bus2',2),(3,'bus2_put2',4);
/*!40000 ALTER TABLE `dispositivo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `info_sensor`
--

DROP TABLE IF EXISTS `info_sensor`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `info_sensor` (
  `idsensor` int NOT NULL AUTO_INCREMENT,
  `tipo` varchar(45) NOT NULL,
  `nombre` varchar(45) NOT NULL,
  `last_value1` float NOT NULL,
  `last_value2` float NOT NULL,
  `iddispositivo` int NOT NULL,
  PRIMARY KEY (`idsensor`),
  KEY `dispositivo_idx` (`iddispositivo`),
  CONSTRAINT `iddispositivo` FOREIGN KEY (`iddispositivo`) REFERENCES `dispositivo` (`iddispositivo`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=334 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `info_sensor`
--

LOCK TABLES `info_sensor` WRITE;
/*!40000 ALTER TABLE `info_sensor` DISABLE KEYS */;
INSERT INTO `info_sensor` VALUES (7,'C02','C02',61,61,1),(77,'hum_temp','DHT_11',0,0,1),(333,'hum_temp','DHT_11',23.7,43,1);
/*!40000 ALTER TABLE `info_sensor` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tipo_actuador`
--

DROP TABLE IF EXISTS `tipo_actuador`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tipo_actuador` (
  `idtipo_actuador` int NOT NULL AUTO_INCREMENT,
  `valor` float NOT NULL,
  `modo` int NOT NULL,
  `idactuador` int NOT NULL,
  PRIMARY KEY (`idtipo_actuador`),
  KEY `idactuador_idx` (`idactuador`),
  CONSTRAINT `idactuador` FOREIGN KEY (`idactuador`) REFERENCES `actuador` (`idactuador`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tipo_actuador`
--

LOCK TABLES `tipo_actuador` WRITE;
/*!40000 ALTER TABLE `tipo_actuador` DISABLE KEYS */;
INSERT INTO `tipo_actuador` VALUES (1,35,1,1),(2,30.2,0,2),(3,47.8,1,3);
/*!40000 ALTER TABLE `tipo_actuador` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `tipo_gps`
--

DROP TABLE IF EXISTS `tipo_gps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `tipo_gps` (
  `idtipo_gps` int NOT NULL AUTO_INCREMENT,
  `x` float NOT NULL,
  `y` float NOT NULL,
  `idsensor` int NOT NULL,
  PRIMARY KEY (`idtipo_gps`),
  KEY `idsensor_idx` (`idsensor`),
  CONSTRAINT `idsensor` FOREIGN KEY (`idsensor`) REFERENCES `info_sensor` (`idsensor`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `tipo_gps`
--

LOCK TABLES `tipo_gps` WRITE;
/*!40000 ALTER TABLE `tipo_gps` DISABLE KEYS */;
/*!40000 ALTER TABLE `tipo_gps` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `usuario`
--

DROP TABLE IF EXISTS `usuario`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `usuario` (
  `idusuario` int NOT NULL AUTO_INCREMENT,
  `nombre` varchar(45) NOT NULL,
  `contraseña` varchar(45) NOT NULL,
  `ciudad` varchar(45) NOT NULL,
  PRIMARY KEY (`idusuario`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb3;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `usuario`
--

LOCK TABLES `usuario` WRITE;
/*!40000 ALTER TABLE `usuario` DISABLE KEYS */;
INSERT INTO `usuario` VALUES (1,'ivan_put','ivan','sevilla'),(2,'javi','javi','utrera'),(4,'clara','clara','madrid'),(7,'german_post','germna','berlin');
/*!40000 ALTER TABLE `usuario` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2021-05-25 22:16:29
