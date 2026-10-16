PSC firmware changes
Dec, 15 2014

Changes to allow setting of PSC board configuration to zero out CVTR data when board is not installed:
Starting with PSC firmware Version 11:

-- Update version number

-- Renamed unused EEPROM location 06 from WTSFSW_REGADDR to BRDCONF_REGADDR
	--  Bit 0 will contain configuration flag for the CVTR board (1 means the board is present)
	-- Bit will be set by user using an EEPROM write command

-- created new variable brd_config which gets set once on board startup
	-- in EEPROMInit()  

	//get the stored board configuration
	i2c_getstat1(EEPROMi2c_CH,EEPROM_SADDR,BRDCONF_REGADDR,1, (unsigned char *)i2cdata);
	brd_config = i2cdata&0x01;

-- added a test for brd_config state in datastream()

	
if(brd_config)
	{
		strcat(message,cv_status);			//Write real CVTR data if board is present
		strcat(message,cvt_errflag);
	}
	else
	{
		strcat(message," swg 0 0.00 0.00 cvt 0 0.0 0.0 0 0.0");		//or else put dummy data in here since there is no CVTR board 
		strcat(message," 00000000");
	}


-- added a test to "get_id" command to output message when no CVTR board present

	if(brd_config)
		sprintf(id_message," PSC_fw_rev:%d CVTR_fw_rev:%d",FWRevision,CVTR_REV);
	else
		sprintf(id_message," PSC_fw_rev:%d CVTR_fw_rev: No board installed",FWRevision);

