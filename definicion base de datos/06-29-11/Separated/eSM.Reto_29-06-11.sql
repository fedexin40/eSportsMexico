-- -----------------------------------------------------
-- Table `eSM`.`Reto`
-- -----------------------------------------------------
CREATE  TABLE IF NOT EXISTS `eSM`.`Reto` (
  `id_Reto` INT NOT NULL ,
  `id_Temp` INT NOT NULL ,
  `id_ModalidadTemporada` INT NOT NULL ,
  `Fecha` DATE NOT NULL ,
  `Hora_Ini` TIME NOT NULL ,
  `Hora_Fin` TIME NOT NULL ,
  `Estatus` INT NOT NULL ,
  PRIMARY KEY (`id_Reto`) ,
  INDEX `Pruebas` (`id_Reto` ASC) ,
  INDEX `ModalidadTemporada` (`id_ModalidadTemporada` ASC) ,
  INDEX `Reglas` () ,
  CONSTRAINT `Pruebas`
    FOREIGN KEY (`id_Reto`)
    REFERENCES `eSM`.`Pruebas` (`id_Reto`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;