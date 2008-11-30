*
* The source code of this program is taken from the FLUKA manual, the USRBIN section
*
      PROGRAM RDBIN
*----------------------------------------------------------------------*
*     Up to MXUSBN user defined binnings are allowed                                    *
*     itusbn = type of binning (must be decoded if .ge. 10)                          *
*     idusbn = distribution to be scored: usual values allowed                       *
*     titusb = binning name                                                          *
*     nxbin   = number of x (r for RZ) intervals                                     *
*     nybin   = number of y (1 for RZ) intervals                                     *
*     nzbin   = number of z intervals                                                *
*     xlow/high = minimum and maximum x (r          for R-Phi-Z)                        *
*     ylow/high = minimum and maximum y (phi for R-Phi-Z)                               *
*     zlow/high = minimum and maximum z                                                 *
*     dxusbn = x (r) bin width                                                       *
*     dyusbn = y (Phi) bin width                                                     *
*     dzusbn = z bin width                                                           *
*     TCUSBN = time cut-off (seconds) for this binning                               *
*     bkusbn = 1st Birk's law parameter for this binning                             *
*     (meaningful only for energy scoring)                                *
*     b2usbn = 2nd Birk's law parameter for this binning              *
*             (meaningful only for energy scoring)                    *
*     xaxusb = x-axis offset for R-Z binning (not possible for R-Phi-Z)*
*     yaxusb = y-axis offset for R-Z binning (not possible for R-Phi-Z)*
**----------------------------------------------------------------------*
      PARAMETER ( MXUSBN = 100 ) ! max. number of binnings
      PARAMETER ( MXSCOR = 500000 ) ! storage for results
      LOGICAL LUSBIN, LUSEVT, LUSTKB
      CHARACTER RUNTIT*80, RUNTIM*32, TITUSB*10, FILNAM*80, CHSTAT*10
      DIMENSION MB(MXUSBN),XLOW(MXUSBN), XHIGH(MXUSBN), YLOW(MXUSBN),
     &     YHIGH (MXUSBN),ZLOW  (MXUSBN), ZHIGH (MXUSBN),DXUSBN(MXUSBN),
     &     DYUSBN(MXUSBN),DZUSBN(MXUSBN), TCUSBN(MXUSBN),BKUSBN(MXUSBN),
     &     B2USBN(MXUSBN),NXBIN (MXUSBN), NYBIN (MXUSBN),NZBIN (MXUSBN),
     &     ITUSBN(MXUSBN),IDUSBN(MXUSBN), KBUSBN(MXUSBN),IPUSBN(MXUSBN),
     &     LEVTBN(MXUSBN),LNTZER(MXUSBN), LTRKBN(MXUSBN),TITUSB(MXUSBN),
     &     XAXUSB(MXUSBN),YAXUSB (MXUSBN), SCORED(MXSCOR)
      WRITE(*,*) ' Type the name of the input file:'
      READ (*,'(A)') FILNAM
      LQ = INDEX(FILNAM,' ') - 1
      OPEN (UNIT=1, FILE=FILNAM, STATUS='OLD', FORM='UNFORMATTED')
      OPEN (UNIT=2, FILE=FILNAM(1:LQ)//'.txt', STATUS='NEW')
*-----------read and write 1st record ---------------------------------
      READ (1) RUNTIT, RUNTIM, WEIPRI, NCASE
      WRITE(2,100) RUNTIT, RUNTIM, NCASE, WEIPRI
*-----------loop on binning detector data in the present file ---------
      DO 1 IB = 1, MXUSBN
         NB = IB
*     ---------------- read and write 2nd record --------------------
         READ (1,END=1000) MB(NB), TITUSB(NB), ITUSBN(NB), IDUSBN(NB),
     &        XLOW(NB), XHIGH(NB), NXBIN(NB), DXUSBN(NB), YLOW(NB),
     &        YHIGH(NB), NYBIN(NB), DYUSBN(NB), ZLOW(NB), ZHIGH(NB),
     &        NZBIN(NB), DZUSBN(NB), LNTZER(NB), BKUSBN(NB), B2USBN(NB),
     &        TCUSBN(NB)
         ITUHLP = MOD (ITUSBN(NB),10)
         IF ( ITUHLP .EQ. 2) THEN
*     Region binning
            NBIN = MAX(NXBIN(NB),NYBIN(NB),NZBIN(NB))
            IR1A = NINT(XLOW(NB))
            IR1B = NINT(XHIGH(NB))
            IDR1 = NINT(DXUSBN(NB))
            IR2A = NINT(YLOW(NB))
            IR2B = NINT(YHIGH(NB))
            IDR2 = NINT(DYUSBN(NB))
            IR3A = NINT(ZLOW(NB))
            IR3B = NINT(ZHIGH(NB))
            IDR3 = NINT(DZUSBN(NB))
            READ(1) (SCORED(J), J = 1, NBIN)
            WRITE(2,101) MB(NB), TITUSB(NB), IDUSBN(NB), NBIN,
     &           IR1A, IR1B, IDR1, IR2A, IR2B, IDR2, IR3A, IR3B, IDR3
            DO 2 I = 1, NBIN
               WRITE(2,1010) IR1A + (I-1)*IDR1, IR2A + (I-1)*IDR2,
     &              IR3A + (I-1)*IDR3, SCORED(I)
 2          CONTINUE
         ELSE IF ( ITUHLP .EQ. 8 ) THEN
*     Region/Lattice/User binning
            IR1A = NINT(XLOW(NB))
            IR1B = NINT(XHIGH(NB))
            IDR1 = NINT(DXUSBN(NB))
            
            IR2A = NINT(YLOW(NB))
            IR2B = NINT(YHIGH(NB))
            IDR2 = NINT(DYUSBN(NB))
            READ(1) (SCORED(J), J = 1, NXBIN(NB)*NYBIN(NB)*NZBIN(NB))
            WRITE(2,102) MB(NB), TITUSB(NB), IDUSBN(NB), NXBIN(NB),
     &           IR1A, IR1B, IDR1, IR2A, IR2B, IDR2, ZLOW(NB),ZHIGH(NB),
     &           NZBIN(NB), DZUSBN(NB)
            J = 0
            IR1 = IR1A
            IR2 = IR2A
            UVAR = ZLOW(NB)
            DO 3 IZ = 1, NZBIN(NB)
               DO 4 IY = 1, NYBIN(NB)
                  DO 5 IX = 1, NXBIN(NB)
                     J = J + 1
                     WRITE(2,1020) IR1, IR1 + IDR1, IR2, IR2 + IDR2,
     &                    UVAR, UVAR + DZUSBN(NB), SCORED(J)
                     IR1 = IR1 + IDR1
 5                CONTINUE
                  IR1 = IR1A
                  IR2 = IR2 + IDR2
                  WRITE(2,*)
 4             CONTINUE
               IR2 = IR2A
               UVAR = UVAR + DZUSBN(NB)
               WRITE(2,*)
 3          CONTINUE
         ELSE IF ((ITUHLP.EQ.1.OR.ITUHLP.EQ.7).AND.NYBIN(NB).LT.2) THEN
*     R-Z binning
*     XAXUSB(NB) = YLOW (NB)
*     YAXUSB(NB) = YHIGH(NB)
            NBIN = NXBIN(NB) * NZBIN(NB)
            READ(1) (SCORED(J), J = 1, NBIN)
            WRITE(2,103) MB(NB), TITUSB(NB), IDUSBN(NB), XLOW(NB),
     &           XHIGH(NB), NXBIN(NB),  DXUSBN(NB), ZLOW(NB), ZHIGH(NB),
     &           NZBIN(NB), DZUSBN(NB) !, XAXUSB(NB), YAXUSB(NB)
            J = 0
            RR = XLOW(NB)
            ZZ = ZLOW(NB)
            DO 6 IZ = 1, NZBIN(NB)
               DO 7 IX = 1, NXBIN(NB)
                  J = J + 1
                  WRITE(2,1030) RR, RR+DXUSBN(NB), ZZ, ZZ+DZUSBN(NB),
     &                 SCORED(J)
                  RR = RR + DXUSBN(NB)
 7             CONTINUE
               RR = XLOW(NB)
               ZZ = ZZ + DZUSBN(NB)
               WRITE(2,*)
 6          CONTINUE
         ELSE IF ( ITUHLP.EQ.1.OR.ITUHLP.EQ.7 ) THEN
*     R-Phi-Z binning
            NBIN = NXBIN(NB) * NYBIN(NB) * NZBIN(NB)
            READ(1) (SCORED(J), J = 1, NBIN)
            WRITE(2,104) MB(NB), TITUSB(NB), IDUSBN(NB), XLOW(NB),
     &           XHIGH(NB), NXBIN(NB), DXUSBN(NB), YLOW(NB), YHIGH(NB),
     &           NYBIN(NB), DYUSBN(NB), ZLOW(NB),  ZHIGH(NB),NZBIN(NB),
     &           DZUSBN(NB)
            J = 0
            RR = XLOW(NB)
            PH = YLOW(NB)
            ZZ = ZLOW(NB)
            DO 8 IZ = 1, NZBIN(NB)
               DO 9 IY = 1, NYBIN(NB)
                  DO 10 IX = 1, NXBIN(NB)
                     J = J + 1
                     WRITE(2,1040) RR, RR + DXUSBN(NB), PH,
     &                    PH + DYUSBN(NB), ZZ, ZZ + DZUSBN(NB),SCORED(J)
                     RR = RR + DXUSBN(NB)
 10               CONTINUE
                  RR = XLOW(NB)
                  PH = PH + DYUSBN(NB)
                  WRITE(2,*)
 9             CONTINUE
               PH = YLOW(NB)
               ZZ = ZZ + DZUSBN(NB)
               WRITE(2,*)
 8          CONTINUE
         ELSE IF ( ITUHLP .EQ. 0 ) THEN
*     Cartesian binning
            NBIN = NXBIN(NB) * NYBIN(NB) * NZBIN(NB)
            READ(1) (SCORED(J), J = 1, NBIN)
            write(2, *) 'test: ' , SCORED(1)
            WRITE(2,105) MB(NB), TITUSB(NB), IDUSBN(NB), XLOW(NB),
     &           XHIGH(NB), NXBIN(NB), DXUSBN(NB), YLOW(NB), YHIGH(NB),
     &           NYBIN(NB), DYUSBN(NB), ZLOW(NB),   ZHIGH(NB),NZBIN(NB),
     &           DZUSBN(NB)
            J = 0
            XX = XLOW(NB)
            YY = YLOW(NB)
            ZZ = ZLOW(NB)
            DO 11 IZ = 1, NZBIN(NB)
               DO 12 IY = 1, NYBIN(NB)
                  DO 13 IX = 1, NXBIN(NB)
                     J = J + 1
                     WRITE(2,1040) XX, XX + DXUSBN(NB), YY,
     &                    YY + DYUSBN(NB), ZZ, ZZ + DZUSBN(NB),SCORED(J)
                     XX = XX + DXUSBN(NB)
 13               CONTINUE
                  XX = XLOW(NB)
                  YY = YY + DYUSBN(NB)
                  WRITE(2,*)
 12            CONTINUE
               YY = YLOW(NB)
               ZZ = ZZ + DZUSBN(NB)
               WRITE(2,*)
 11         CONTINUE
         END IF
 1    CONTINUE                  ! end loop on detectors
*     ------------------------------------------------------------------
 1000 CONTINUE
 100  FORMAT(/,1X,'*****',2X,A80,2X,'*****',/,/,10X,A32,/,/,
     &     10X,'Total number of particles followed ',I9,', for a ',
     &     'total weight of ',1P,E15.8,/)
 101  FORMAT (/, 3X, 'Region    binning n. ', I3, '   "', A10,
     &     '" , generalised particle n. ', I4, /, 6X, I5,
     &     'bins corresponding to the region sets:' /,6X,'from region ',
     &     I5, ' to region ', I5, ' in step of ', I5, ' regions, or', /,
     &     6X, 'from region ', I5, ' to region ', I5,' in step of ', I5,
     &     ' regions, or', /, 6X, 'from region ', I5, ' to region ', I5,
     &     ' in step of ', I5, ' regions',//, 7X, 'Set 1', 5X, 'Set 2',
     &     5X, 'Set 3', 6X, 'Score',/)
 102  FORMAT (/, 3X, 'Reg/Lat/U binning n. ', I3, '   "', A10,
     &     '" , generalised particle n. ',I4, /, 6X, I5,
      
     &     ' bins corresponding to the region-related set:' /, 6X,
     &     'from region ', I5, ' to region ', I5, ' in step of ', I5,
     &     ' regions, and', /, 6X, I5,
     &     ' bins corresponding to the lattice-related set:' /, 6X,
     &     'from lattice', I5, ' to lattice', I5, ' in step of ', I5,
     &     ' lattices, and', /, 6X, 'U coordinate: from ', 1P, E11.4,
     &     ' to ', E11.4, ' ux, ', 0P, I5, ' bins (', 1P, E11.4,
     &     ' ux wide)', /)
 103  FORMAT('1', /, 3X, 'R - Z binning n. ', I3, ' "', A10,
     &     '" , generalised particle n. ',I4,/,6X,'R coordinate: from ',
     &     1P,E11.4,' to ',E11.4, ' cm, ', 0P, I5, ' bins (', 1P, E11.4,
     &     ' cm wide)', /, 6X, 'Z coordinate: from ', 1P, E11.4, ' to ',
     &     E11.4, ' cm, ', 0P,I5, ' bins (', 1P, E11.4, ' cm wide)', //,
     &     ' between R1',2X, 'and', 5X,'R2',13X,'between Z1', 2X, 'and',
     &     5X, 'Z2', 15X, 'Score', /)
*     & 6X, 'axis coordinates: X =', 1P, E11.4, ', Y = ', E11.4, ' cm'/
 104  FORMAT('1', /, 3X, 'R-Phi-Z binning n. ', I3, ' "', A10,
     &     '" , generalised particle n. ', I4,/, 6X,
     &     'R coordinate: from ',1P, E11.4, ' to ', E11.4, ' cm, ', 0P,
     &     I5, ' bins (', 1P, E11.4, ' cm wide)', /, 6X,
     &     'Phi coordinate: from ', 1P, E11.4, ' to ',E11.4,' rad, ',0P,
     &     I5, ' bins (', 1P, E11.4, ' rad wide)', /, 6X,
     &     'Z coordinate: from ', 1P, E11.4, ' to ', E11.4, ' cm,', 0P,
     &     I5, ' bins (', 1P, E11.4, ' cm wide)', //, ' between R1',1X,
     &     'and', 4X, 'R2', 11X, 'between Phi1', 1X, 'and', 2X, 'Phi2',
     &     10X, 'between Z1', 1X, 'and', 4X, 'Z2', 13X, 'Score', /)
 105  FORMAT('1', /, 3X, 'Cartesian binning n. ', I3, ' "', A10,
     &     '" , generalised particle n. ',I4,/,6X,'X coordinate: from ',
     &     1P, E11.4, ' to ', E11.4, ' cm,', 0P,I5,' bins (', 1P, E11.4,
     &     ' cm wide)', /, 6X, 'Y coordinate: from ', 1P, E11.4, ' to ',
     &     E11.4, ' cm, ', 0P, I5, ' bins (',1P,E11.4, ' cm wide)',/,6X,
     &     'Z coordinate: from ', 1P, E11.4, ' to ',E11.4,' cm, ',0P,I5,
     &     ' bins (', 1P, E11.4, ' cm wide)' //, ' between X1',1X,
     &     'and', 4X, 'X2', 12X, 'between Y1', 1X, 'and', 3X, 'Y2',
     &     12X, 'between Z1', 1X, 'and', 4X, 'Z2', 13X, 'Score', /)
 1010 FORMAT(3I10, 5X, 1P, E11.4)
 1020 FORMAT(I11, 4X, I11, 5X, I11, 4X, I11, 5X, 1P, E11.4, 4X, E11.4,
     &     6X, E11.4)
 1030 FORMAT(1P, E11.4, 5X, E11.4, 6X, E11.4, 5X, E11.4, 8X, E11.4)
 1040 FORMAT(1P, E11.4, 4X, E11.4, 5X, E11.4, 4X, E11.4, 5X, E11.4, 4X,
     &     E11.4, 6X, E11.4)
      END
      
