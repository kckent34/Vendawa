//
//  FilloutViewController.swift
//  DoctorApp
//
//  Created by Casey Kent on 2/14/16.
//  Copyright © 2016 Casey Kent. All rights reserved.
//

import UIKit
import MessageUI

class FilloutViewController: UIViewController
{

    var medicine:String?
    var machine:Machine?
    var patientNumber:String?
    var first:String?
    var last:String?
    var quantity:String?
    let messageComposer = MessageComposer()
    var codeString:String?
    
    @IBOutlet weak var titleLabel: UILabel!
    @IBOutlet weak var firstNameTextField: UITextField!
    @IBOutlet weak var lastNameTextField: UITextField!
    @IBOutlet weak var phoneNumberTextField: UITextField!
    @IBOutlet weak var QuantityTextField: UITextField!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        print(machine?.location)
        print(medicine!)
        titleLabel.text = "Prescribing " + medicine! + " to " + (machine?.location)! + " APM"
        // Do any additional setup after loading the view.
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
  
    @IBAction func sendToPatientButtonPressed(sender: UIButton) {
        if (messageComposer.canSendText()) {
            // Obtain a configured MFMessageComposeViewController
            messageComposer.recipient = phoneNumberTextField.text!
            
            messageComposer.first = firstNameTextField.text!
            messageComposer.last = lastNameTextField.text!
            messageComposer.med = medicine!
            
            messageComposer.machineLocation = machine?.location
            
            messageComposer.type = 1
            let messageComposeVC = messageComposer.configuredMessageComposeViewController()
            
            
            
            // Present the configured MFMessageComposeViewController instance
            // Note that the dismissal of the VC will be handled by the messageComposer instance,
            // since it implements the appropriate delegate call-back
            presentViewController(messageComposeVC, animated: true, completion: nil)
        } else {
            // Let the user know if his/her device isn't able to send text messages
            let errorAlert = UIAlertView(title: "Cannot Send Text Message", message: "Your device is not able to send text messages.", delegate: self, cancelButtonTitle: "OK")
            errorAlert.show()
        }
        
    }
    @IBAction func sendButtonPressed(sender: UIButton) {
        
        if (messageComposer.canSendText()) {
            // Obtain a configured MFMessageComposeViewController
            messageComposer.recipient = machine?.phoneNumber
            messageComposer.first = firstNameTextField.text!
            messageComposer.last = lastNameTextField.text!
            messageComposer.med = medicine!
            messageComposer.type = 0;
            
            let code = Int(arc4random_uniform(8)+1)*1000 + Int(arc4random_uniform(8)+1)*100 + Int(arc4random_uniform(8)+1)*10 + Int(arc4random_uniform(8)+1)
            
            codeString = String(code) + String("#")
            
            messageComposer.codeString = codeString
            let messageComposeVC = messageComposer.configuredMessageComposeViewController()
            
    
            
            // Present the configured MFMessageComposeViewController instance
            // Note that the dismissal of the VC will be handled by the messageComposer instance,
            // since it implements the appropriate delegate call-back
            presentViewController(messageComposeVC, animated: true, completion: nil)
        } else {
            // Let the user know if his/her device isn't able to send text messages
            let errorAlert = UIAlertView(title: "Cannot Send Text Message", message: "Your device is not able to send text messages.", delegate: self, cancelButtonTitle: "OK")
            errorAlert.show()
        }
      
        
    }
    
   
    
    


   
    // MARK: - Navigation

    // In a storyboard-based application, you will often want to do a little preparation before navigation
    override func prepareForSegue(segue: UIStoryboardSegue, sender: AnyObject?) {
        // Get the new view controller using segue.destinationViewController.
        // Pass the selected object to the new view controller.
    }


}
