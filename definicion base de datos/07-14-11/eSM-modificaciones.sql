SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE SCHEMA IF NOT EXISTS `mydb` DEFAULT CHARACTER SET latin1 COLLATE latin1_swedish_ci ;

USE `mydb`;

CREATE  TABLE IF NOT EXISTS `eSM`.`Disponibilidad_Horas` (
  `id_Jugador` INT(11) NOT NULL ,
  `Morning` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Evening` TINYINT(4) NOT NULL DEFAULT 0 ,
  `Night` TINYINT(4) NULL DEFAULT 0 ,
  PRIMARY KEY (`id_Jugador`) ,
  UNIQUE INDEX `id_Agente_Libre_UNIQUE` (`id_Jugador` ASC) ,
  INDEX `fk_Disponibilidad_Horas_Agente_Libre` (`id_Jugador` ASC) ,
  CONSTRAINT `fk_Disponibilidad_Horas_Agente_Libre`
    FOREIGN KEY (`id_Jugador` )
    REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
    ON DELETE NO ACTION
    ON UPDATE CASCADE)
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

ALTER TABLE `eSM`.`Agente_Libre` DROP COLUMN `id_Agente_Libre` 
, ADD UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) 
, DROP INDEX `id_Agente_Libre_UNIQUE` 
, DROP PRIMARY KEY 
, ADD PRIMARY KEY (`id_Jugador`) ;

ALTER TABLE `eSM`.`Disponibilidad` ADD COLUMN `Domingo` TINYINT(4) NOT NULL DEFAULT 0  AFTER `Martes` , ADD COLUMN `Jueves` TINYINT(4) NOT NULL DEFAULT 0  AFTER `Martes` , ADD COLUMN `Miercoles` TINYINT(4) NOT NULL DEFAULT 0  AFTER `Martes` , ADD COLUMN `Sabado` TINYINT(4) NOT NULL DEFAULT 0  AFTER `Jueves` , ADD COLUMN `Viernes` TINYINT(4) NOT NULL DEFAULT 0  AFTER `Jueves` , CHANGE COLUMN `id_Agente_Libre` `id_Jugador` INT(11) NOT NULL  , CHANGE COLUMN `Fecha_Inicio` `Lunes` TINYINT(4) NOT NULL DEFAULT 0  , CHANGE COLUMN `Fecha_Fin` `Martes` TINYINT(4) NOT NULL DEFAULT 0  , DROP FOREIGN KEY `fk_Fechas_Disponibles_Agente_Libre` ;

ALTER TABLE `eSM`.`Disponibilidad` 
  ADD CONSTRAINT `fk_Dias_Disponibles_Agente_Libre`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE
, ADD UNIQUE INDEX `id_Jugador_UNIQUE` (`id_Jugador` ASC) 
, DROP INDEX `Fechas_Disponibles_UNIQUE` 
, DROP INDEX `Fecha_Fin_UNIQUE` 
, DROP PRIMARY KEY 
, ADD PRIMARY KEY (`id_Jugador`) 
, DROP INDEX `fk_Fechas_Disponibles_Agente_Libre` 
, ADD INDEX `fk_Fechas_Disponibles_Agente_Libre` (`id_Jugador` ASC) , RENAME TO  `eSM`.`Disponibilidad_Dias` ;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
