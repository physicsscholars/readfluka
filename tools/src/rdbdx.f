      PROGRAM RDBDX
* ... from the FLUKA manual...
*----------------------------------------------------------------------
* Up to MXUSBX user-defined bdrx detectors are allowed
*
*    ABXHGH = maximum angle (steradian) (WHAT(4) of 2nd card)
*    ABXLOW = minimum angle (steradian) (WHAT(5) of 2nd card)
*    AUSBDX = area (cm**2) of the detector (WHAT(6) of first card)
*    DABXBN = angular bin width (steradian) if linear in angle,
*             otherwise ratio between upper and lower edge of angular
*             intervals
*    DEBXBN = energy bin width (GeV) if linear in energy or referring
*             to a low-energy neutron energy group, otherwise
*             ratio between upper and lower edge of energy intervals
*    EBXHGH = maximum energy (GeV) (WHAT(1) of 2nd card)
*    EBXLOW = minimum energy (GeV) (WHAT(2) of 2nd card)
*    ENGMAX = upper energies (GeV) of the neutron groups
*    IDUSBX = (generalized) particle scored (WHAT(2) of first card)
*    IGMUSX = maximum low-energy neutron group to be scored
*    ITUSBX = type of binning (i1 in WHAT(1) of first card):
*              1 = linear energy, linear angle
*              2 = linear energy, logarithmic angle,
*             -1 = logarithmic energy, linear angle,
*             -2 = logarithmic energy, logarithmic angle
*    LFUSBX = current if .F., fluence if .T. (i3 in WHAT(1) of card 1)
*    LLNUSX = no low-energy neutron scoring if false, yes if true
*    LWUSBX = one way if false, 2-ways if true (i2 in WHAT(1) of card 1)
*    MX     = id-number of the boundary crossing detector
*    NABXBN = number of angular intervals (WHAT(6) of 2nd card)
*    NCASE = number of beam particles followed
*    NEBXBN = number of energy intervals
*    NR1USX = first region (WHAT(4) of first card)
*    NR2USX = second region (WHAT(5) of first card)
*    RUNTIM = date and time of the run (as printed on standard output)
*    RUNTIT = title of the run (as given by card TITLE)
*    SCORED = result array
*    TITUSX = detector name (SDUM in first USBDRX card)
*    WEIPRI = total weight of primaries
*    NHIGH = number of energy bins above low-energy neutron limit
*    CUMULE = energy-cumulative fluence/current
*    CUMULA = energy-angle-cumulative fluence/current
*    ELIMIT = lower energy of the first bin above or straddling the
*             n-group limit
*
*----------------------------------------------------------------------
      PARAMETER ( MXUSBX = 1500 )      ! max. number of usbdrx detectors
      PARAMETER ( MXENER = 1000)       ! storage for results
      PARAMETER ( MXANGL = 100)        !
      PARAMETER ( MXSCOR = MXENER*MXANGL )
      PARAMETER ( NMXGRP = 100 )        ! # of low-energy neutron groups
      LOGICAL LFUSBX, LWUSBX, LLNUSX
      CHARACTER RUNTIT*80,RUNTIM*32,TITUSX*10,FILNAM*80
      DIMENSION  EBXLOW(MXUSBX), EBXHGH(MXUSBX), ABXLOW(MXUSBX),
     &           ABXHGH(MXUSBX), DEBXBN(MXUSBX), DABXBN(MXUSBX),
     &           AUSBDX(MXUSBX), NEBXBN(MXUSBX), NABXBN(MXUSBX),
     &           NR1USX(MXUSBX), NR2USX(MXUSBX), ITUSBX(MXUSBX),
     &           IDUSBX(MXUSBX), IGMUSX(MXUSBX), LFUSBX(MXUSBX),
     &           LWUSBX(MXUSBX), LLNUSX(MXUSBX), TITUSX(MXUSBX),
     &           ENGMAX (NMXGRP+1), SCORED(MXSCOR), ELIMIT(MXUSBX),
     &           MX(MXUSBX), NHIGH(MXUSBX)
      DOUBLE PRECISION CUMUL, ANGINT, EN1, EN2, ELIMIT, DIFF
      WRITE(*,*)' Type the name of the input file:'
      READ (*,'(A)') FILNAM
      LQ = INDEX(FILNAM,' ') - 1
      OPEN (UNIT=1, FILE=FILNAM, STATUS='OLD', FORM='UNFORMATTED')
      OPEN (UNIT=2, FILE=FILNAM(1:LQ)//'.txt', STATUS='NEW')
*----------- read and write 1st record ---------------------------------
      READ (1) RUNTIT,RUNTIM,WEIPRI,NCASE
      WRITE(2,100) RUNTIT, RUNTIM, NCASE, WEIPRI
 100  FORMAT(/,1X,'*****',2X,A80,2X,'*****',/,/,10X,A32,/,/,
     &        10X,'Total number of particles followed ',I9,', for a ',
     &        'total weight of ',1P,E15.8,/)
*----------- loop on bdryx detector data in the present file -----------
      DO 1 IX = 1, MXUSBX
         NX = IX
*        ---------------- read and write 2nd record --------------------
         READ (1, END=1000) MX(NX), TITUSX(NX), ITUSBX(NX), IDUSBX(NX),
     &      NR1USX(NX), NR2USX(NX), AUSBDX(NX), LWUSBX(NX), LFUSBX(NX),
     &      LLNUSX(NX), EBXLOW(NX), EBXHGH(NX), NEBXBN(NX), DEBXBN(NX),
     &      ABXLOW(NX), ABXHGH(NX), NABXBN(NX), DABXBN(NX)
         WRITE(2,101) MX(NX),
     &     TITUSX(NX), IDUSBX(NX), NR1USX(NX), NR2USX(NX), AUSBDX(NX)
*        ------------ if low-en. neutrons, read group energies ---------
         IF ( LLNUSX (NX) ) THEN
            READ (1) IGMUSX(NX), (ENGMAX(IG), IG = 1, IGMUSX(NX)+1)
            WRITE (2,102) IGMUSX(NX)
         ELSE
            IGMUSX(NX) = 0
         END IF
*        ---------------------- 1 or 2-way -----------------------------
         IF ( LWUSBX (NX) ) THEN
            WRITE (2,'(6X,A)') 'this is a two ways estimator'
         ELSE
            WRITE (2,'(6X,A)') 'this is a one way only estimator'
         END IF
*        ---------------------- fluence or current ---------------------
         IF ( LFUSBX (NX) ) THEN
            WRITE (2,'(6X,A)') 'this is a fluence-like estimator'
         ELSE
            WRITE (2,'(6X,A)') 'this is a current-like estimator'
         END IF
*        -------------------- linear or log in energy ------------------
         IF ( ITUSBX (NX) .GT. 0 ) THEN
            WRITE (2,103) EBXLOW(NX), EBXHGH(NX), NEBXBN(NX), DEBXBN(NX)
         ELSE
            WRITE (2,104) EBXLOW(NX), EBXHGH(NX), NEBXBN(NX), DEBXBN(NX)
         END IF
*        -------------------- linear or log in angle -------------------
         IF ( ABS (ITUSBX (NX)) .LE. 1 ) THEN
            WRITE (2,105) ABXLOW(NX), ABXHGH(NX), NABXBN(NX), DABXBN(NX)
         ELSE
            WRITE (2,106) ABXLOW(NX), ABXHGH(NX), NABXBN(NX), DABXBN(NX)
         END IF
*        interv = total number of energy intervals
*        (intervals above the limit for n-groups + intervals below)
         INTERV = NEBXBN(NX) + IGMUSX(NX)
*------------ read the scoring results as a 1-dimensional array --------
         READ (1) (SCORED(J), J = 1, INTERV * NABXBN(NX))
*------------ loop on angles -------------------------------------------
         CUMUL = 0.D0
         DO 2 IA = 1, NABXBN(NX)
            IF ( ABS (ITUSBX (NX)) .LE. 1 ) THEN
*               linear in angle
               WRITE(2,107) IA, ABXLOW(NX) + (IA-1)*DABXBN(NX),
     &                        ABXLOW(NX) + IA*DABXBN(NX)
            ELSE
*               logarithmic in angle
               IF(IA .EQ. 1) THEN
                   WRITE (2,108) ABXLOW(NX)
               ELSE
                   WRITE (2,107) IA, ABXLOW(NX) * DABXBN(NX)**(IA-1),
     &                                ABXLOW(NX) * DABXBN(NX)**IA
               END IF
            END IF
            WRITE (2,'("                              ",
     &        "        Double Differential    Angle-Integrated",
     &        "           Cumulative" )')
            IF ( LFUSBX (NX) ) THEN
               WRITE (2,'("                               ",
     &                    "     Fluence (dPhi/dE/dOmega)       dPhi/dE ",
     &                    "               Fluence")')
            ELSE
               WRITE (2,'("                               ",
     &                      "     Current (dJ/dE/dOmega)       dJ/dE ",
     &                      "             Current")')
            END IF
            WRITE (2,'("Lower energy        Upper energy",
     &                  "     cm**-2 GeV**-1 sr-1        cm**-2 GeV-1",
     &                  "              cm**-2"/)')
            ELIMIT(NX) = EBXLOW(NX)
*           -------------------- low-energy neutrons -------------------
            IF ( LLNUSX (NX) ) THEN
*               low-energy neutron data, if present, are stored backwards
               IG = IGMUSX(NX)
               EN1 = ENGMAX(IG+1)
*               ------------- loop on low-energy groups -----------------
               DO 3 JG = IA*(NEBXBN(NX)+IGMUSX(NX)),
     &                    IA*(NEBXBN(NX)+IGMUSX(NX))-IGMUSX(NX)+1,-1
                   EN2 = ENGMAX(IG)
                   ANGINT = SCORED(JG) * DABXBN(NX)
                   CUMUL = CUMUL + ANGINT * (EN2 - EN1)
                   WRITE(2,109) EN1, EN2, SCORED(JG), ANGINT, CUMUL
                   IG = IG - 1
                   EN1 = EN2
 3              CONTINUE                  ! end loop on low-energy groups
*               find lower limit of first bin above or straddling the
*               n-group limit. Nhigh: counts the high energy bins
               NHIGH(NX) = 0
*        for the time being, set energy boundary at n-group limit
         ELIMIT(NX) = EN1
         EN1 = EBXHGH(NX)
         DO 4 IE = 1, NEBXBN(NX)
            IF(ITUSBX(NX) .GT. 0) THEN
               EN2 = EN1 - DEBXBN(NX)
            ELSE
               EN2 = EN1 / DEBXBN(NX)
            END IF
            EN1 = EN2
            NHIGH(NX) = NHIGH(NX) + 1
            IF(EN2 .LE. ELIMIT(NX)) GO TO 5
 4       CONTINUE
 5       CONTINUE
      ELSE
         EN1 = EBXLOW(NX)
         NHIGH(NX) = NEBXBN(NX)
      END IF
*     ------------------------------------------------------------
*     first bin above or straddling the n-group limit
      IF(ITUSBX(NX) .GT. 0) THEN
         EN2 = EN1 + DEBXBN(NX)
      ELSE
         EN2 = EN1 * DEBXBN(NX)
      END IF
      DIFF = SCORED(IA * (NEBXBN(NX)) - NHIGH(NX) + 1)
      ANGINT = DIFF * DABXBN(NX)
      CUMUL = CUMUL + ANGINT * (EN2 - ELIMIT(NX))
      WRITE(2,109) ELIMIT(NX), EN2, DIFF, ANGINT, CUMUL
      EN1 = EN2
*     -------- loop on energies above the n-group limit ----------
      DO 6 IE = 2, NHIGH(NX)
         IF(ITUSBX(NX) .GT. 0) THEN
            EN2 = EN1 + DEBXBN(NX)
         ELSE
            EN2 = EN1 * DEBXBN(NX)
         END IF
         DIFF = SCORED(IA * (NEBXBN(NX)) - NHIGH(NX) + IE)
         ANGINT = DIFF * DABXBN(NX)
         CUMUL = CUMUL + ANGINT * (EN2 - EN1)
         WRITE(2,109) EN1, EN2, DIFF, ANGINT, CUMUL
         EN1 = EN2
  6   CONTINUE            ! end loop on energies above limit
*     Case of generalized particles .ne. 8 but including neutrons
      IF (LLNUSX(NX) .AND. IDUSBX(NX) .NE. 8 .AND.
     &                      NEBXBN(NX) .GT. NHIGH(NX)) THEN
        WRITE(2,110) ELIMIT(NX)
        EN1 = EBXLOW(NX)
        DO 7 IE = 1, NEBXBN(NX) - NHIGH(NX)
           IF(ITUSBX(NX) .GT. 0) THEN
              EN2 = EN1 + DEBXBN(NX)
           ELSE
              EN2 = EN1 * DEBXBN(NX)
           END IF
               DIFF = SCORED(IA * (NEBXBN(NX)) - NHIGH(NX) + IE)
           DIFF = SCORED((IA-1) * (NEBXBN(NX)+IGMUSX(NX)) + IE)
           ANGINT = DIFF * DABXBN(NX)
           CUMUL = CUMUL + ANGINT * (EN2 - EN1)
           WRITE(2,109) EN1, EN2, DIFF, CUMULE, CUMULA
           EN1 = EN2
 7      CONTINUE
      END IF
*            ------------------------------------------------------------
 2        CONTINUE               ! end loop on angles
*         ---------------------------------------------------------------
 1     CONTINUE                  ! end loop on detectors
*      ------------------------------------------------------------------
 1000 CONTINUE
 101   FORMAT(/,3X,'Bdrx n. ',I3,' "',A10,
     &     '" , generalized particle n. ',I4,', from region n. ',I6,
     &     ' to region n. ',I6,/,6X,'detector area: ',1P,E15.8,' cm**2')
 102   FORMAT(6X,'low-energy neutrons scored from group 1 to group ',I5)
 103   FORMAT(6X,'linear energy binning from ',1P,E11.4,' to ',
     &     E11.4,' GeV, ',0P,I6,' bins (',1P,E11.4,' GeV wide)')
 104   FORMAT(6X,'logar. energy binning from ',1P,E11.4,' to ',
     &     E11.4,' GeV, ',0P,I6,' bins (ratio :',1P,E11.4,')')
 105   FORMAT(6X,'linear angular binning from ',1P,E11.4,' to ',
     &     E11.4,' sr , ',0P,I6,' bins (',1P,E11.4,' sr wide )'/)
 106   FORMAT(6X,'logar. angular binning, first bin = ',1P,E11.4,
     &     ' sr, upper limit ',E11.4,' sr, ',0P,I6,' bins ( ratio :',
     &     1P,E11.4,')'/)
 107   FORMAT(/' Angle ',I4,' between ',1P,E11.4,' and ',E11.4,' sr'/)
 108   FORMAT(/' Angle    1 between 0 and ',1P,E11.4, ' sr'/)
 109   FORMAT(1P, 2(E11.4, 7X), 3(E14.7, 8X))
 110   FORMAT(/,20X,"Particles other than neutrons below E=",1P,E11.4/)
      END



