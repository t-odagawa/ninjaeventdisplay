#cd ${EVENT_DISPLAYDIR}/bin
cd ${HOME}/WagasciEventDisplay/build/src

inputdir=${HOME}/data/hitconv
inputfile=${inputdir}/track_b2physics_wagasci_babymind_2021_ninja_nodead_$1_$2_$3.root

#outputdir=${HOME}/SharingFile/display/output
outputdir=${HOME}/data/plots

hsmoutputdir=/hsm/nu/ninja/pra_event_display/ecc5

geometrydir=${HOME}/Programs/WagasciMC/etc/wagasci/b2/geometry

entry=$4

bunchid=$5

eventid=$6

command=${HOME}/SharingFile/display/input/command.txt

./EventDisplay --input-file ${inputfile} --output-directory ${outputdir} --geometry-directory ${geometrydir} --start-event ${entry} -l pdf -b ${bunchid} < ${command}
mv ${outputdir}/event_display_${entry}.pdf ${hsmoutputdir}event_display_${eventid}.pdf
