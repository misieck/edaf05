package lab1;

import java.util.Arrays;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.Scanner;

public class GaleShapley {

	private static int nbrOfPairs;
	private static int[][] personer;
	private static int[][] företag;

	public static void main(String[] args) {

		inputParser();
		System.out.println(Arrays.deepToString(personer));
		System.out.println(Arrays.deepToString(företag));
		galeShapleyPairing();

		return;
	}

	private static void inputParser() {
		Scanner scan = new Scanner(System.in);
		nbrOfPairs = scan.nextInt(); // Sparar antalet personer och företag

		// Skapa en array för alla personer och deras önskelista
		personer = new int[nbrOfPairs][nbrOfPairs];
		företag = new int[nbrOfPairs][nbrOfPairs];

		HashSet<Integer> passedPersons = new HashSet<Integer>();
		// Läs in datan så länge det finns
		while (scan.hasNextInt()) {
			int individNbr = scan.nextInt();
			boolean hasBeenPassed = !passedPersons.add(individNbr);

			for (int i = 0; i < nbrOfPairs; i++) {
				if (hasBeenPassed) {// Lägger in för person individNbr
					personer[individNbr - 1][i] = scan.nextInt();
				} else {// Lägger in för företag individNbr
					företag[individNbr - 1][i] = scan.nextInt();
				}
			}

			// System.out.println(hasBeenPassed);

		}

		scan.close();
		return;
	}

	private static void galeShapleyPairing() {
		LinkedList<Integer> alonePersons = new LinkedList<Integer>();
		int[][] pairing = new int[nbrOfPairs][2];
		for(int i = 0; i < nbrOfPairs; i++) {//Alla personer i en lista
			alonePersons.add(i);
			pairing[i][0] = -1; //Ingen är parad från början, betecknas med -1
		}
		System.out.println(Arrays.deepToString(pairing));
		while(!alonePersons.isEmpty()) {
			int currentPerson = alonePersons.pop();
			
			
			
		}
		return;
	}
}
