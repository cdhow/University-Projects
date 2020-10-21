import { Component, Input } from '@angular/core';
import { AlertController } from '@ionic/angular';
import { Storage } from '@ionic/storage';

@Component({
  selector: 'app-history',
  templateUrl: './history.component.html',
  styleUrls: ['./history.component.scss'],
})
export class HistoryComponent  {

  // From home parent page component
  @Input() history:number[];

  isItemPositive = false;

  constructor(private alertCtrl:AlertController, private storage:Storage) {}

  async removeItem(index) {
    this.history.splice(index, 1);
    this.storage.set('history', this.history); // update history
  }


  // Prompt user with edit input
  async editItem(index) {
    const prompt = await this.alertCtrl.create({
      inputs: [
        {
          placeholder: "Change Amount",
          name: "historyItem"
        }
      ],
      buttons: [
        {
          text: "Cancel"
        },
        {
          text: "Save",
          handler: data => {
            if (index > -1) {
              this.history[index] = data.historyItem;
              this.storage.set('history', this.history); // update history
            } 
          }
        }
      ]
    });
    await prompt.present();
  }

}
