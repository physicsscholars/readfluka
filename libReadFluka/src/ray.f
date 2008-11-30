*     .................................................................
*       This code is based on the listing shown in Chapter 14 of the FLUKA manual
*       .................................................................
      INCLUDE '(DIMPAR)'
      INCLUDE '(FLKMAT)'
!      INCLUDE '(DBLPRC)'
!      INCLUDE '(SCOHLP)'
!      INCLUDE '(SOUEVT)'

      PARAMETER (LUNRAY = 10)
      CHARACTER MATNAME*8
      CHARACTER FILNAM*80
      INTEGER NRAYRN, MREG, MLATTC, MMAT, MREGLD, MLATLD, MMATLD, IDISC
      REAL EKIN, XX, YY, ZZ, R2, R3, THETAP, PHIPOS, TXX, TYY, TZZ,
     &     THETAD, PHIDIR, ETADIR, RCM, ALAMDI, ALAMDP, ALAMDN, ALAMDG,
     &     ALAMDR, DEKMIP, GMOCM2, DELAGE, RCMTOT, ALITOT, ALPTOT,
     &     ALNTOT, ALGTOT, ALRTOT, TOTMIP, SRHTOT, AGEAGE
*     .................................................................
*     here other possible declarations
*     .................................................................
      WRITE (*,*) ' File name?'
      READ (*,'(A80)') FILNAM
      OPEN (FILE = FILNAM, UNIT = LUNRAY, STATUS = 'OLD', FORM =
     & 'UNFORMATTED')
*     loop over several rays
 1    CONTINUE
*     read info about ray starting point
      READ (LUNRAY, END = 1000) NRAYRN, MREG, MLATTC, MMAT, EKIN
      READ (LUNRAY, END = 1000) XX, YY, ZZ, R2, R3, THETAP, PHIPOS
      READ (LUNRAY, END = 1000) TXX, TYY, TZZ, THETAD, PHIDIR, ETADIR
*     ................................................................
*     here possible user code to manipulate values read
!      print *, TXX, TYY, TZZ, XX, YY, ZZ
      print *, "starting region: ", MREG
      print *, "starting lattice cell: ", MLATTC
      print *, "material of starting region: ", MMAT
      print *, "reference kinetic energy of the ray: ", EKIN, "GeV"
      write(*, "(a,a)") " reg lat  material       density",
     &     "   z [cm]  TOTMIP  ALAMDR  ALRTOT  GMOCM2 SRHTOT"
*     ................................................................
*     loop over further positions along the ray path
 2    CONTINUE
*     read info about next point
      READ (LUNRAY, END = 3000) MREGLD, MLATLD, MMATLD,
     &                                 MATNAME, IDISC
      READ (LUNRAY, END = 2000) XX, YY, ZZ, R2, R3, THETAP, PHIPOS
      READ (LUNRAY, END = 2000) RCM, ALAMDI, ALAMDP, ALAMDN, ALAMDG,
     &                                 ALAMDR, DEKMIP, GMOCM2, DELAGE
      READ (LUNRAY, END = 2000) RCMTOT, ALITOT, ALPTOT, ALNTOT,
     &                                 ALGTOT, ALRTOT, TOTMIP, SRHTOT,
     &                                 AGEAGE
*     .............................................................
*     possible user code to manipulate values read
      write(*, "(i4,i4,a10,i4,f9.3,f9.3,f8.3,f8.3, f8.3, f8.2, f8.2)"),
     &     MREGLD, MLATLD, MATNAME, MMATLD, RHO(MMATLD), ZZ,
     &     TOTMIP, ALAMDR, ALRTOT, GMOCM2, SRHTOT
*     .............................................................
      IF ( IDISC .EQ. 0 ) THEN
*     ...........................................................
*     possible user code at the end of ray step
*     ...........................................................
         GO TO 2
      END IF
*     ...............................................................
*     possible user code at the end of ray trajectory
*     ...............................................................
*     new ray
      GO TO 1
 1000 CONTINUE
      WRITE(*,*) ' Incomplete data on file about ray starting point'
      GO TO 3000
 2000 CONTINUE
      WRITE(*,*) ' Incomplete data on file about ray trajectory'
 3000 CONTINUE
*     .................................................................
*     possible user code at the end of analysis
*     .................................................................
      CLOSE (UNIT = LUNRAY)
      END
      
