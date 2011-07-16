SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='TRADITIONAL';

CREATE  TABLE IF NOT EXISTS `eSM`.`Puntos_Desafio_Jugador_Inferior` (
  `Diferencia` INT(11) NOT NULL DEFAULT 0 ,
  `Puntos_Win` INT(11) NOT NULL DEFAULT 0 ,
  `Puntos_Loss` INT(11) NOT NULL DEFAULT 0 ,
  INDEX `Puntos_Desafio_Jugador_Inferior` (`Diferencia` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Puntos_Desafio_Jugador_Superior` (
  `Diferencia_Puntos` INT(11) NOT NULL DEFAULT 0 ,
  `Puntos_Win` INT(11) NOT NULL DEFAULT 0 ,
  `Puntos_Loss` INT(11) NOT NULL DEFAULT 0 ,
  INDEX `Puntos_Desafio_Jugador_Superior` (`Diferencia_Puntos` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

CREATE  TABLE IF NOT EXISTS `eSM`.`Puntuacion_Torneos` (
  `Inscritos` INT(11) NOT NULL DEFAULT 0 ,
  `1` INT(11) NOT NULL DEFAULT 0 ,
  `2` INT(11) NOT NULL DEFAULT 0 ,
  `4` INT(11) NOT NULL DEFAULT 0 ,
  `8` INT(11) NOT NULL DEFAULT 0 ,
  `16` INT(11) NOT NULL DEFAULT 0 ,
  `32` INT(11) NOT NULL DEFAULT 0 ,
  `64` INT(11) NOT NULL DEFAULT 0 ,
  `128` INT(11) NOT NULL DEFAULT 0 ,
  `256` INT(11) NOT NULL DEFAULT 0 ,
  `512` INT(11) NOT NULL DEFAULT 0 ,
  `1024` INT(11) NOT NULL DEFAULT 0 ,
  INDEX `Torneos_Jugadores_Inscritos` (`Inscritos` ASC) )
ENGINE = InnoDB
DEFAULT CHARACTER SET = latin1
COLLATE = latin1_swedish_ci;

ALTER TABLE `eSM`.`Puntuacion` CHANGE COLUMN `Puntuacion` `Puntuacion` INT(11) NOT NULL DEFAULT 0  , CHANGE COLUMN `PJ` `PJ` INT(11) NOT NULL DEFAULT 0  , CHANGE COLUMN `PG` `PG` INT(11) NOT NULL DEFAULT 0  , CHANGE COLUMN `Racha` `Racha` INT(11) NOT NULL DEFAULT 0  ;

ALTER TABLE `eSM`.`Integrantes_Equipo` DROP COLUMN `Mensaje` , DROP FOREIGN KEY `fk_Integrantes_Equipo_Equipo` ;

ALTER TABLE `eSM`.`Integrantes_Equipo` 
  ADD CONSTRAINT `fk_Integrantes_Equipo_Equipo`
  FOREIGN KEY (`nid_Equipo` )
  REFERENCES `eSM`.`Equipo` (`nid_Equipo` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE
, ADD UNIQUE INDEX `Estado_UNIQUE` (`Estado` ASC) 
, ADD PRIMARY KEY (`nid_Equipo`) ;

ALTER TABLE `eSM`.`Agente_Libre` DROP FOREIGN KEY `fk_Agente_Libre_Jugador` ;

ALTER TABLE `eSM`.`Agente_Libre` 
  ADD CONSTRAINT `fk_Agente_Libre_Jugador`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Jugador` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE;

ALTER TABLE `eSM`.`Disponibilidad_Horas` DROP FOREIGN KEY `fk_Disponibilidad_Horas_Agente_Libre` ;

ALTER TABLE `eSM`.`Disponibilidad_Horas` 
  ADD CONSTRAINT `fk_Disponibilidad_Horas_Agente_Libre`
  FOREIGN KEY (`id_Jugador` )
  REFERENCES `eSM`.`Agente_Libre` (`id_Jugador` )
  ON DELETE NO ACTION
  ON UPDATE CASCADE;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
