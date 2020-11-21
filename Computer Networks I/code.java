
package diktia;
import ithakimodem.Modem;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.lang.System;
import java.util.Timer;
import java.util.*;
import java.lang.Math;
/*
*
* Δίκτυα Υπολογιστών I
*
* Experimental Virtual Lab
*
* Java virtual modem communications seed code
*
*/
public class virtualModem {
public static void main(String[] param) throws IOException {
(new virtualModem()).demo();
}
public void demo() throws IOException {
int k;
boolean b;
Modem modem;
modem = new Modem();
modem.setSpeed(80000);
modem.setTimeout(1000);
modem.open("ithaki");
// κωδικοι αποθηκευμενοι σε πινακες byte
byte[] echo = "E1349\r".getBytes();
byte[] imgErF = "M3690\r".getBytes();
byte[] imgEr = "G1761\r".getBytes();
byte[] gps = "P7854R=1000199\r".getBytes();
byte[] ack = "Q5159\r".getBytes();
byte[] nack = "R7207\r".getBytes();

PrintWriter responseTimesEcho = new
PrintWriter("responseTimesEcho.txt", "UTF-8");
FileOutputStream imageErrorFree = new FileOutputStream("E1.jpg");
FileOutputStream imageError = new FileOutputStream("E2.jpg");
FileOutputStream imageGPS = new FileOutputStream("M1.jpg");
// first dial
for (int i = 0; i < 205; i++) {
try {
k = modem.read();
if (k == -1)
break;
System.out.print((char) k);
} catch (Exception x) {
break;
}
}
long timeRun = System.currentTimeMillis();
long endRun = timeRun + 300000;
// echo packets
while (System.currentTimeMillis() < endRun) {
responseTimesEcho.println(echoPackets(echo, modem));
}
responseTimesEcho.close();
// images
images(imgErF, modem, imageErrorFree);
imageErrorFree.close();
images(imgEr, modem, imageError);
imageError.close();

// GPS ME STIGMATA
int map[][] = new int[99][80];
map = gps(modem, gps);
long T[] = new long[5];
T = gpsT(map);
String codeForPins = "P7854T=" + T[4];
// εισαγωγη των τιμων του πινακα Τ με τα σημεια που θα παρουμε
στιγματα σε ενα string ωστε με κληση της getBytes παρακατω να παρουμε πινακα
bytes
for (int i = 0; i < 4; i++) {
codeForPins += "T=" + T[3 - i];
}
codeForPins += "\r";
System.out.println(codeForPins);
byte[] gpsPins = codeForPins.getBytes();
images(gpsPins, modem, imageGPS);
imageGPS.close();
// εδω γινονται μερικα read μεχρι να παρουμε timeout για να μην
υπαρχει λαθος στην επομενη διεργασια
for (;;) {
try {
k = modem.read();
if (k == -1)
break;
System.out.print((char) k);
} catch (Exception x) {
break;
}
}
// ARQ
long ArqTime = System.currentTimeMillis();
long ArqEndTime = ArqTime + 300000;
PrintWriter responseTimesForArq = new
PrintWriter("responseArq.txt", "UTF-8");
PrintWriter NumOfRetransmissions = new
PrintWriter("retransmissions.txt", "UTF-8");
int errors = 0;
int numOfpackets = 0;
int[] stats = new int[2];

// αφηνουμε τη συναρτηση να παιρνει πακετα μεχρι ενα
προκαθορισμενο χρονικο διαστημα
while (System.currentTimeMillis() < ArqEndTime) {
stats = arqFunc(modem, ack, nack, responseTimesForArq,
NumOfRetransmissions);
errors += stats[1];
numOfpackets += stats[0];
}
System.out.println(errors);
System.out.println(numOfpackets);
responseTimesForArq.close();
NumOfRetransmissions.close();
// NOTE : Break endless loop by catching sequence "\r\n\n\n".
// NOTE : Stop program execution when "NO CARRIER" is detected.
// NOTE : A time-out option will enhance program behavior.
// NOTE : Continue with further Java code.
// NOTE : Enjoy :)
modem.close();
}
public int echoPackets(byte[] echoReq, Modem modem1) {
modem1.write(echoReq);
int k;
int counter = 0;
long startT = System.currentTimeMillis();// χρονος εκκινησης του
προγραμματος
long endT = 0;
long responseT;
//καλουμε τη read 35 φορες οσες το μηκος του καθε πακετου
for (counter = 0; counter < 35; counter++) {
try {
k = modem1.read();
if (k == -1) {
break;
}
System.out.print((char) k);
} catch (Exception x) {
break;
}
}
endT = System.currentTimeMillis();

responseT = endT - startT;//ευρεση του χρονου αποκρισης με
αφαιρεση των χρονικων στιγμων πριν και μετα το περας της διεργασιας για καθε
πακετο
return (int) responseT;
}
public void images(byte[] imageErFr, Modem m, FileOutputStream imgErrF)
throws IOException {
m.write(imageErFr);
//μεταβλητες για τον ελεγχο του τελους της εικονας
int last = 0;
int preLast = 0;
int n = 0;//μεταβλητη που θα μας δειχνει αν εχουμε μπει μεσα στο
if αλλαζοντας την και τσεκαροντας την τιμη της ε καποιο σημειο του
προγραμματος
// βροχος while για να τσεκαρουμε ποτε ξεκινανε τα byte εναρξης
της εικονας ffd8 ή 255216
while (true) {
preLast = last;
last = m.read();
if (last == -1)
break;
if (preLast == 255 && last == 216) {
n = 1;//αλλαγη της τιμης της n
imgErrF.write(255);
imgErrF.write(216);
//βροχος που σταματα μονο οταν φτασουμε στα bytes
ληξης της εικονας
while (true) {
try {
last = m.read();
// συνθηκη που ελεγχει αν εχουμε φτασει
στα bytes ληξης της εικονας
if (preLast == 255 && last == 217) {
imgErrF.write((byte) last);
break;
}
preLast = last;
imgErrF.write((byte) last);
if (last == -1)
break;
}

catch (Exception x) {
break;
}
}
// ελεγχος της n ωστε να τερματιστει ο εξωτερικος
βροχος
if (n == 1)
break;
}
}
imgErrF.close();
}
public int[][] gps(Modem m, byte gps1[]) {
int stars = 0;
m.write(gps1);
int k = 0;
int packetsMap[][] = new int[99][80];
int i = -1; // αρχικοποιειται με -1 διοτι παρακατω μολις μπει
στην if εαν δει δολαριο
// γινεται αμεσως αυξηση του
int j = 0;
for (int l = 0; l < 27; l++) { // εδω πεταμε το START ITHAKI
ΚΤΛ..
try {
k = m.read();
if (k == -1) {
break;
}
System.out.print((char) k);
} catch (Exception x) {
break;
}
}
for (;;) { // εδω βαζουμε σε εναν πινακα ολα τα ιχνη
try {
k = m.read();
if (k == 42)//με την καταμετριση των αστερακιων
ουσιαστικα μετραμε το ποσα πακετα εχουμε παρει
stars++;
if (stars == 99)

break; // σταματαμε οταν δουμε το τελευταιο
αστερακι αρα πηραμε το τελευταιο πακετο και
// δεν πηραμε το μηνυμα Stop
ithaki κτλ
// αυξηση του i κατα ενα ωστε να προχωρησουμε στην
επομενη γραμμη καθως βλεπουμε οτι ξεκιναει το επομενο ιχνος με τον χακακρητα $
if (k == 36) {
i++;
j = 0;
}
packetsMap[i][j] = k;
j++;
if (k == -1) {
break;
}
System.out.print((char) k);
} catch (Exception x) {
break;
}
}
return packetsMap;// επιστρεφουμε τον πινακα με τα ιχνη που
αποθηκευσαμε στο τελευταιο for,οπου καθε γραμμη
}
public long[] gpsT(int[][] pckt) {
int p = 0;
int[][] arrayT = new int[99][17];// αποθηκευει μονο τις
συντεταγμενες απο τα ιχνη
int[] arrayP = new int[99];
int[] arrayM = new int[99];
int[][] finalValues = new int[99][10];
String[] arrayTstring = new String[99];
for (int i = 0; i < 99; i++) {// σε αυτη τη φορ αποθηκευουμε σε
int τις τιμες των συντεταγμενων
p = 0; // χρησιμοποιειται για να μην παρεμβαλεται κενος
χαρακτηρας αναμεσα στο μηκος και στο πλατος
// long-ξεκιναμε απο τη 18η και σταματαμε
στην 26 θεση γτ εκει ξεκιναν και τελειωνουν οι συντεταγμενες
for (int j = 18; j < 27; j++) {
if (pckt[i][j] == 46)// για αποφυγη της τελειας
{
p = 1;
continue;

}
arrayT[i][j - 18 - p] = pckt[i][j] - 48;// στη θεση
48 ειναι το 0,και ετσι γινεται μετατροπη σε δεκαδικο συστημα των ascii τιμων
}
p = 0;
// lat
for (int j = 30; j < 40; j++) {
if (pckt[i][j] == 46) {
p = 1;
continue;
}
arrayT[i][j - 22 - p] = pckt[i][j] - 48;
}
}
String[] strT = new String[99];
for (int i = 0; i < 99; i++) {
strT[i] = "";
for (int j = 0; j < 17; j++) {
strT[i] += Integer.toString(arrayT[i][j]);// βαζουμε
σε πινακα string τις συντεταγμενες,ε με μετατροπη των ακεραιων σε string
}
arrayP[i] = (int) (Integer.parseInt(strT[i].substring(4,
8)) * 0.006);// typecast για να κρατησω τα 2 πρωτα ψηφια που θελω απο την
μετατροπη
// των
δευτερολεπτων του πλατους
arrayM[i] = (int) (Integer.parseInt(strT[i].substring(13,
17)) * 0.006);//ομοια για το μηκος
}
int suitablePos[] = new int[5];// κραταει τις θεσεις των ιχνων
που μας κανουν
suitablePos[0] = 0;
int indexPos = 0;
int t = 1;
// στον πινακα finalValues 10 θεσεων εχω σε 4 πρωτες θεσεις τα 4
στοιχεια του
// μηκους που δεν πειραχτηκαν καιστην 5η(αντιστοιχα στην 10η) το
κομματι που
// μετετρεψα το οποιο καταλαμβανει μια θεση αν και μπορει να εχει
παραπανω απο

// ενα ψηφια
for (int i = 0; i < 99; i++) {
for (int j = 0; j < 4; j++) {
finalValues[i][j] = arrayT[i][j + 9];
}
finalValues[i][4] = arrayM[i];
for (int h = 5; h < 9; h++) {
finalValues[i][h] = arrayT[i][h - 5];
}
finalValues[i][9] = arrayP[i];
}
int timeValues[] = new int[99];// πινακας με τους χρονους των
πακετων σε δευτερολεπτα
for (int i = 0; i < 99; i++) {
timeValues[i] = ((pckt[i][7] - 48) * 10 + (pckt[i][8] -
48)) * 3600
+ ((pckt[i][9] - 48) * 10 + (pckt[i][10] -
48)) * 60
+ ((pckt[i][11] - 48) * 10 + (pckt[i][12] -
48));
}
// επιλεγουμε να παρουμε ιχνη των οποιων η χρονικη διαφορα ειναι
μεγαλυτερη απο 17,ωστε να μην πεσουμε σε ιδια ιχνη
for (int i = 0; i < 99; i++) {
if (Math.abs(timeValues[indexPos] - timeValues[i]) >= 17)
{
indexPos = i;
suitablePos[t] = i;
t++;
}
if (t == 5) {
break;
}
}
long[] T = new long[5];
String[] str2 = new String[5];
int oneCellperNum[] = new int[5];
System.out.println();
System.out.println(suitablePos[0]);
System.out.println(suitablePos[1]);
System.out.println(suitablePos[2]);
System.out.println(suitablePos[3]);
System.out.println(suitablePos[4]);
for (int i = 0; i < 5; i++) {
str2[i] = "";
for (int j = 0; j < 10; j++) {

str2[i] +=
Integer.toString(finalValues[suitablePos[i]][j]);// βαζουμε σε πινακα string
τις
// συντεταγμενες
}
T[i] = (long) Long.parseLong(str2[i]);
}
return T;// επιστροφη του πινακα με τα ιχνη
}
public int[] arqFunc(Modem modem1, byte[] ack_code, byte[] nack_code,
PrintWriter txt1, PrintWriter txt2) {
modem1.write(ack_code);
int k = 0;
int counter = 0;
long startT = System.currentTimeMillis();// χρονος εκκινησης του
προγραμματος
long endT = 0;
long responseT;
int arrayPck[] = new int[58];
int seq[] = new int[16];
int valueOfXor;
int valueOfFcs;
int[] stats = new int[2];
for (;;) {
try {
for (counter = 0; counter < 58; counter++) {
k = modem1.read();
arrayPck[counter] = k; // τοποθετηση του
πακετου σε εναν πινακα
}
for (int j = 0; j < 16; j++) {
seq[j] = arrayPck[31 + j];// σε αυτο το
σημειο παιρνουμε τον 16αδικο κωδικο
}
// υπολογισμος του xor του κομματιου των 16
χαρακτηρων
valueOfXor = seq[0] ^ seq[1];
for (int l = 0; l < 14; l++) {
valueOfXor = valueOfXor ^ seq[l + 2];
}
String str1;
str1 = "";

// ενωση των ψηφιων του fcs με μετατροπη σε string
και μετα παλι σε integer
for (int i = 0; i < 3; i++) {
str1 += Integer.toString(arrayPck[49 + i] -
48);
}
valueOfFcs = Integer.parseInt(str1);
// ελεγχος για το αν πηραμε το σωστο πακετο
τσεκαροντας την τιμη xor του 16αδικου και του fcs
if (valueOfFcs == valueOfXor) {
for (int j = 0; j < 58; j++) {
System.out.print((char) arrayPck[j]);
}
// test
stats[0]++;
System.out.println();
System.out.println("valueOfFcs = " +
valueOfFcs + "valueOfXor = " + valueOfXor);
break;
}
// σε περιπτωση που οι 2 αριθμοι που ελεγχτηκαν δεν
ταυτιζονται ζητουμε επανακληση με του κωδικου nack
else {
modem1.write(nack_code);
stats[1]++;
System.out.println(" wronggggg" + "valueOfFcs
= " + valueOfFcs + "valueOfXor = " + valueOfXor);
}
if (k == -1) {
break;
}
} catch (Exception x) {
break;
}
}
endT = System.currentTimeMillis();
responseT = endT - startT;
txt1.println(responseT);
txt2.println(stats[1]);

return stats;// επιστροφη ενος πινακα που στην πρωτη θεση εχουμε
τον αριθμο των σωστων πακετων και στη δευτερη θεση τις επανακλησεις που
χρειαστηκε να κανουμε ωσπου να ερθει το σωστο πακετο
}
}