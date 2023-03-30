package lab1;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.Scanner;

public class GaleShapley {

	private static int nbrOfPairs;
	private static int[][] personer;
	private static int[][] scores;
	//private static int[][] scores;

	public static void main(String[] args) {

		inputParser();
		// System.out.println(Arrays.deepToString(personer));
		// System.out.println(Arrays.deepToString(företag));
		galeShapleyPairing();

		return;
	}

	private static void inputParser() {
		//System.out.println("Start loopin");

		Scanner scan = new Scanner(System.in);
		nbrOfPairs = scan.nextInt(); // Sparar antalet personer och företag

		// Skapa en array för alla personer och deras önskelista
		personer = new int[nbrOfPairs][nbrOfPairs];
		scores = new int[nbrOfPairs][nbrOfPairs];

		HashSet<Integer> passedPersons = new HashSet<Integer>();
		// Läs in datan så länge det finns

		long totalTime = 0;
		long count = 0;
		while (scan.hasNextInt()) {
			int individNbr = scan.nextInt();

			boolean hasBeenPassed = !passedPersons.add(individNbr);

			for (int i = 0; i < nbrOfPairs; i++) {

				long timeA = java.lang.System.nanoTime();
				int nextInt =  scan.nextInt();
				count ++;
				long timeB = java.lang.System.nanoTime();
				totalTime = totalTime + (timeB - timeA);

				if (hasBeenPassed) {// Lägger in för person individNbr

					personer[individNbr - 1][i] = nextInt - 1;
				} else {// Lägger in för företag individNbr
					scores[individNbr - 1][nextInt-1] = i;//nextInt - 1;
				}
			}
		}


		scan.close();
		//System.out.println("Done  parsin, count: " + count + ", total time: " + totalTime/1000000);
		return;
	}

	private static void galeShapleyPairing() {
		LinkedList<Integer> alonePersons = new LinkedList<Integer>();
		int[] pairingC = new int[nbrOfPairs]; // Företag index
		int[] pairingP = new int[nbrOfPairs]; // Företag index

		boolean[][] applied = new boolean[nbrOfPairs][nbrOfPairs];
		for (int i = 0; i < nbrOfPairs; i++) {// Alla personer i en lista
			alonePersons.add(i);
			pairingC[i] = -1;
			pairingP[i] = -1;
		}


		int[] counters = new int[nbrOfPairs];
		while (!alonePersons.isEmpty()) {
			int currentPerson = alonePersons.pop();

			int companyPrioI = personer[currentPerson][counters[currentPerson]];

			if (pairingC[companyPrioI] == -1) {
				addPairing(pairingC, pairingP, companyPrioI, currentPerson);
			} else if (companyPrefersAOverB(companyPrioI, currentPerson, pairingC[companyPrioI])) {
				int previousPerson = replacePairing(pairingC, pairingP, companyPrioI, currentPerson);
				alonePersons.addLast(previousPerson);
			} else {
				alonePersons.addLast(currentPerson);
			}
			counters[currentPerson]++;
		}
		
		for (int i = 0; i < nbrOfPairs; i++) {
			System.out.println(pairingC[i] + 1);

		}
		return;
	}

	private static void addPairing(int[] pairingC,  int[] pairingP, int company, int person) {
		if (pairingC[company] != -1) throw new AssertionError();
		pairingC[company] = person;
		pairingP[person] = company;
		return;
	}

	private static int replacePairing(int[] pairingC, int[] pairingP, int company, int person) {
		int ret = pairingC[company];
		int otherCompany = pairingP[company];
		if (otherCompany == company) throw new AssertionError();
		pairingP[person] = -1;
		pairingC[company] = person;
		return ret;
	}

	private static boolean companyPrefersAOverB(int company, int personA, int personB) {

		return scores[company][personA] < scores[company][personB];
	}
}
