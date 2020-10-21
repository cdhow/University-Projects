import { Component, OnInit } from '@angular/core';
import { TermsPage } from './terms/terms.page';

import { ModalController, AlertController } from '@ionic/angular';
import { Storage } from '@ionic/storage';

@Component({
  selector: 'app-home',
  templateUrl: 'home.page.html',
  styleUrls: ['home.page.scss'],
})

export class HomePage implements OnInit {

  betAmount: number = null;
  finalAmount: number = null;
  dateTime: Date = null;

  history: number[] = [];  // Recent History
  results: number[] = [];  // Results for results page
  overallResults:number;   // Diplayed on results page
  presentTerms: boolean;

  constructor(private modalController: ModalController, private alertCtrl: AlertController, private storage: Storage) {}

  async ngOnInit() {
    // Default T&A to present to true
    this.presentTerms = await this.storage.get('T&A');
    if (this.presentTerms == null) this.presentTerms = true;

    this.overallResults = await this.storage.get('oResults');
    if (this.overallResults == null) this.overallResults = 0; // Intially 0

    this.history = await this.storage.get('history');
  }

  calcHistory() {
    let result = this.finalAmount - this.betAmount;
    // Add result to history
    if (this.history.length > 3)
      this.history.length = 0;
    if (result != 0) {
      this.history.push(result);  // history is used in child component "history"
      this.storage.set('history', this.history); // update history in storage
    }
  }

  calcResult() { 
    let result = this.finalAmount - this.betAmount;
    this.overallResults += result;
    // Update overall results
    this.storage.set('oResults', this.overallResults);
  }

  // Present terms once then Send results to history and Results page
  async sendResults() {
    // Initially present terms
    if (this.presentTerms) {
      const modal = await this.modalController.create({
        component: TermsPage
      })
      this.presentTerms = false;
      this.storage.set("T&A", this.presentTerms);
      return modal.present();
    } else {
      // Terms have been accepted, proccess data
      // Resest input fields to empty (null)
      this.calcHistory();
      this.calcResult();
      this.finalAmount = null;
      this.betAmount = null;

    }
  }



}
