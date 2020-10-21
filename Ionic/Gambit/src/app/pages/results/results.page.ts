import { Component, OnInit } from '@angular/core';
import { Storage } from '@ionic/storage';

@Component({
  selector: 'app-results',
  templateUrl: './results.page.html',
  styleUrls: ['./results.page.scss'],
})

export class ResultsPage implements OnInit {

  results: number;
  month = 'April';
  year = '2019';

  rdValue:string = 'bar'; // Toggling which chart to display

  constructor(private storage: Storage) {}

  ngOnInit() {}

  ionViewDidEnter() {
    // Display overall results
    this.storage.get('oResults').then((retval) => {
      this.results = retval;
    });
      }

  }

