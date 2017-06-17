
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.util.Scanner;
import javax.swing.*;
import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;

public class Application extends JFrame {

    public JButton GetObject;
    public JTextField InputFile;
    public JTextField OuputDirectory;
    public JButton SaveObject;
    public JCheckBox CatStaticObjects;
    public JButton Center;
    
    public static void main(String[] args) {
        
        new Application();
        
    }
    
    public Application() {
        
        this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        this.setSize(700, 300);
        this.setTitle("Center *.abal objects");
        this.setLocationRelativeTo(null);
        this.setResizable(false);
        
        JPanel thePanel = new JPanel();
        
        InputFile = new JTextField(40);
        thePanel.add(InputFile);
        
        GetObject = new JButton("Browse");
        myActionListener al = new myActionListener();
        GetObject.addActionListener(al);
        thePanel.add(GetObject);
        
        OuputDirectory = new JTextField(40);
        thePanel.add(OuputDirectory);
        
        SaveObject = new JButton("Browse");
        thePanel.add(SaveObject);
        
        CatStaticObjects = new JCheckBox("Concatenate static objects?");
        thePanel.add(CatStaticObjects);
        
        JPanel ButtonPanel = new JPanel();
        
        Center = new JButton("Center Objects");
        ButtonPanel.add(Center);

        thePanel.add(ButtonPanel);

        this.add(thePanel);
        this.setVisible(true);
        
    }
    
    public class myActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            if (e.getSource() == GetObject) {
                JFileChooser OpenDialog = new JFileChooser();
                OpenDialog.setDialogType(JFileChooser.OPEN_DIALOG);
                myFileFilter ff = new myFileFilter();
                OpenDialog.setFileFilter(ff);
                OpenDialog.setAcceptAllFileFilterUsed(true);
                FileFilter ExtensionFF = new 
        FileNameExtensionFilter("Apa Borsec Asset List (*.abal)", "abal");
                OpenDialog.addChoosableFileFilter(ExtensionFF);
                OpenDialog.setVisible(true);
                int option = OpenDialog.showOpenDialog(null);
                if (option == JFileChooser.APPROVE_OPTION) {
                    InputFile.setText(OpenDialog.getSelectedFile().getAbsolutePath());
                }
            }
        }
        
    }
    
    public class myFileFilter extends FileFilter {

        @Override
        public boolean accept(File file) {
            return file.toString().endsWith("abal") ||
                    file.isDirectory();
        }

        @Override
        public String getDescription() {
            return "";
        }
        
    }
    
}
